#!/usr/bin/env python3
"""Run the real Garbox LVGL UI in a local interactive browser window."""

from __future__ import annotations

import argparse
import json
import shutil
import subprocess
import sys
import threading
import time
import webbrowser
from http import HTTPStatus
from http.server import BaseHTTPRequestHandler, ThreadingHTTPServer
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
PROGRAM = ROOT / ".pio" / "build" / "ui-simulator" / "program"
WIDTH = 320
HEIGHT = 240


PAGE = r"""<!doctype html>
<html lang="en">
<head>
  <meta charset="utf-8">
  <meta name="viewport" content="width=device-width,initial-scale=1">
  <title>Garbox UI simulator</title>
  <style>
    :root { color-scheme: dark; font-family: Inter, ui-sans-serif, system-ui, sans-serif; }
    * { box-sizing: border-box; }
    body { margin: 0; min-height: 100vh; display: grid; place-items: center; color: #e7ecef;
      background: radial-gradient(circle at 50% 10%, #283039 0, #12171c 45%, #080b0e 100%); }
    main { width: min(1050px, 96vw); padding: 24px 18px 32px; }
    header { display: flex; align-items: end; justify-content: space-between; gap: 18px; margin-bottom: 14px; }
    h1 { font-size: 18px; font-weight: 650; letter-spacing: .04em; margin: 0; }
    #status { color: #8ea0ac; font: 12px ui-monospace, SFMono-Regular, monospace; text-align: right; }
    .unit { padding: clamp(12px, 2vw, 24px); border: 1px solid #3b444c; border-radius: 24px;
      background: linear-gradient(145deg, #22292f, #11161a); box-shadow: 0 28px 80px #0009, inset 0 1px #ffffff15; }
    .display-bezel { max-width: 970px; margin: auto; padding: 9px; border: 1px solid #53616a; border-radius: 12px;
      background: #252d33; box-shadow: inset 0 1px #ffffff12, 0 8px 24px #0007; }
    .display-caption { display: flex; justify-content: space-between; padding: 0 2px 7px;
      color: #9baab3; font: 10px ui-monospace, SFMono-Regular, monospace; letter-spacing: .12em; }
    .display-frame { border: 2px solid #8296a1; border-radius: 5px; overflow: hidden;
      background: #070b0e; box-shadow: 0 0 0 1px #000, inset 0 0 0 1px #ffffff12; }
    canvas { display: block; width: 100%; aspect-ratio: 4 / 3; background: #000; image-rendering: pixelated;
      touch-action: none; }
    .controls { display: grid; grid-template-columns: 1fr auto 1fr; align-items: center; gap: clamp(14px, 4vw, 48px);
      max-width: 690px; margin: 25px auto 4px; }
    button { -webkit-tap-highlight-color: transparent; user-select: none; color: #dfe6e9; cursor: pointer;
      border: 1px solid #4a555e; background: linear-gradient(#303940, #1b2227); box-shadow: 0 5px 10px #0008, inset 0 1px #ffffff1c; }
    button:active, button.active { transform: translateY(2px); background: #171d21; box-shadow: 0 2px 5px #0009, inset 0 2px 5px #0008; }
    .turn { min-height: 58px; border-radius: 13px; font-size: 22px; }
    .encoder { width: 112px; height: 112px; border-radius: 50%; position: relative; font-size: 11px; letter-spacing: .1em;
      text-transform: uppercase; touch-action: none; }
    .encoder::before { content: ""; position: absolute; left: 50%; top: 9px; width: 3px; height: 18px; border-radius: 2px;
      background: #b8c7ce; transform: translateX(-50%); }
    .help { margin: 18px auto 0; max-width: 760px; color: #84929b; font-size: 12px; line-height: 1.55; text-align: center; }
    kbd { border: 1px solid #4a555e; border-bottom-width: 2px; border-radius: 4px; padding: 1px 5px; color: #b9c5cb; }
    .error { color: #ff867c !important; }
    @media (max-width: 600px) { main { padding-inline: 9px; } .unit { border-radius: 14px; } .encoder { width: 84px; height: 84px; } }
  </style>
</head>
<body>
<main>
  <header><h1>GARBOX // LIVE LVGL</h1><div id="status">connecting…</div></header>
  <section class="unit">
    <div class="display-bezel">
      <div class="display-caption"><span>DISPLAY OUTPUT</span><span>320 × 240</span></div>
      <div class="display-frame"><canvas id="screen" width="320" height="240"></canvas></div>
    </div>
    <div class="controls">
      <button class="turn" id="ccw" title="Rotate counter-clockwise">↶</button>
      <button class="encoder" id="encoder">press</button>
      <button class="turn" id="cw" title="Rotate clockwise">↷</button>
    </div>
    <div class="help">
      Rotate with the arrows, mouse wheel, or <kbd>←</kbd>/<kbd>→</kbd>. Click the encoder for a short press;
      hold it for 0.7 s for a long press. <kbd>Space</kbd> and <kbd>Enter</kbd> also operate the encoder button.
    </div>
  </section>
</main>
<script>
  const canvas = document.querySelector('#screen');
  const ctx = canvas.getContext('2d', {alpha: false});
  const status = document.querySelector('#status');
  let framePending = false;
  let inputPending = Promise.resolve();

  function send(action, value = 0) {
    inputPending = inputPending.then(() => fetch('/input', {
      method: 'POST', headers: {'Content-Type': 'application/json'},
      body: JSON.stringify({action, value})
    })).catch(showError);
    return inputPending;
  }

  function rotate(value) { send('rotate', value); }
  document.querySelector('#ccw').addEventListener('click', () => rotate(-1));
  document.querySelector('#cw').addEventListener('click', () => rotate(1));

  const encoder = document.querySelector('#encoder');
  let held = false;
  let longTimer = null;
  function press(event) {
    if (held) return;
    event?.preventDefault();
    held = true;
    encoder.classList.add('active');
    if (event?.pointerId !== undefined) encoder.setPointerCapture?.(event.pointerId);
    send('press');
    longTimer = setTimeout(() => send('long'), 700);
  }
  function release(event) {
    if (!held) return;
    event?.preventDefault();
    held = false;
    encoder.classList.remove('active');
    clearTimeout(longTimer);
    send('release');
  }
  encoder.addEventListener('pointerdown', press);
  encoder.addEventListener('pointerup', release);
  encoder.addEventListener('pointercancel', release);

  let wheelReady = true;
  canvas.addEventListener('wheel', event => {
    event.preventDefault();
    if (!wheelReady) return;
    wheelReady = false;
    rotate(event.deltaY < 0 ? 1 : -1);
    setTimeout(() => wheelReady = true, 55);
  }, {passive: false});

  function isEncoderKey(event) {
    return event.code === 'Space' || event.key === 'Enter';
  }

  addEventListener('keydown', event => {
    if (event.repeat) return;
    if (event.key === 'ArrowLeft' || event.key.toLowerCase() === 'a') rotate(-1);
    else if (event.key === 'ArrowRight' || event.key.toLowerCase() === 'd') rotate(1);
    else if (isEncoderKey(event)) press(event);
  });
  addEventListener('keyup', event => {
    if (isEncoderKey(event)) release(event);
  });

  function showError(error) {
    status.textContent = 'simulator disconnected';
    status.classList.add('error');
    console.error(error);
  }

  async function refresh() {
    if (framePending) return;
    framePending = true;
    try {
      const response = await fetch('/frame', {cache: 'no-store'});
      if (!response.ok) throw new Error(await response.text());
      const rgb = new Uint8Array(await response.arrayBuffer());
      if (rgb.length !== 320 * 240 * 3) throw new Error('invalid framebuffer length');
      const image = ctx.createImageData(320, 240);
      for (let source = 0, target = 0; source < rgb.length; source += 3, target += 4) {
        image.data[target] = rgb[source];
        image.data[target + 1] = rgb[source + 1];
        image.data[target + 2] = rgb[source + 2];
        image.data[target + 3] = 255;
      }
      ctx.putImageData(image, 0, 0);
      const power = response.headers.get('X-Garbox-Power') === '1' ? 'ON' : 'OFF';
      const fan = Math.round(Number(response.headers.get('X-Garbox-Fan-Output')) * 100);
      status.textContent = `${response.headers.get('X-Garbox-Screen')} · ${response.headers.get('X-Garbox-Temperature')} °C` +
        ` → ${response.headers.get('X-Garbox-Target')} °C · ${power} · fan ${fan}%`;
      status.classList.remove('error');
    } catch (error) { showError(error); }
    finally { framePending = false; }
  }
  setInterval(refresh, 66);
  refresh();
</script>
</body>
</html>
"""


class Simulator:
    def __init__(self, executable: Path):
        self._lock = threading.Lock()
        self._process = subprocess.Popen(
            [str(executable), "--interactive"],
            cwd=ROOT,
            stdin=subprocess.PIPE,
            stdout=subprocess.PIPE,
            stderr=None,
            bufsize=0,
        )
        ready = self._readline()
        if not ready.startswith("READY "):
            raise RuntimeError(f"simulator did not start: {ready}")

    def _readline(self) -> str:
        assert self._process.stdout is not None
        line = self._process.stdout.readline()
        if not line:
            raise RuntimeError(f"simulator exited with code {self._process.poll()}")
        return line.decode("ascii").rstrip("\n")

    def command(self, command: str) -> None:
        with self._lock:
            self._write(command)
            response = self._readline()
            if response != "OK":
                raise RuntimeError(response)

    def frame(self) -> tuple[bytes, dict[str, str]]:
        with self._lock:
            self._write("frame")
            header = self._readline().split()
            if len(header) != 11 or header[0] != "FRAME":
                raise RuntimeError("invalid frame response: " + " ".join(header))
            length = int(header[1])
            assert self._process.stdout is not None
            data = bytearray()
            while len(data) < length:
                chunk = self._process.stdout.read(length - len(data))
                if not chunk:
                    raise RuntimeError("simulator closed midway through a frame")
                data.extend(chunk)
            return bytes(data), {
                "screen": header[2],
                "temperature": header[3],
                "target": header[4],
                "power": header[5],
                "tick": header[6],
                "fan_auto": header[7],
                "fan_manual": header[8],
                "fan_output": header[9],
                "brightness": header[10],
            }

    def _write(self, command: str) -> None:
        if self._process.poll() is not None:
            raise RuntimeError(f"simulator exited with code {self._process.returncode}")
        assert self._process.stdin is not None
        self._process.stdin.write((command + "\n").encode("ascii"))
        self._process.stdin.flush()

    def close(self) -> None:
        if self._process.poll() is not None:
            return
        try:
            with self._lock:
                self._write("quit")
                self._readline()
        except (BrokenPipeError, RuntimeError):
            pass
        try:
            self._process.wait(timeout=2)
        except subprocess.TimeoutExpired:
            self._process.terminate()
            self._process.wait(timeout=2)


class SimulationTicker:
    def __init__(self, simulator: Simulator):
        self._simulator = simulator
        self._stop = threading.Event()
        self._thread = threading.Thread(target=self._run, name="garbox-ticker", daemon=True)
        self._started = False

    def start(self) -> None:
        self._thread.start()
        self._started = True

    def _run(self) -> None:
        deadline = time.monotonic()
        while not self._stop.is_set():
            deadline += 0.033
            try:
                self._simulator.command("step 1")
            except (BrokenPipeError, RuntimeError) as error:
                print(f"simulator ticker stopped: {error}", file=sys.stderr)
                return
            self._stop.wait(max(0.0, deadline - time.monotonic()))

    def stop(self) -> None:
        self._stop.set()
        if self._started:
            self._thread.join(timeout=2)


class Handler(BaseHTTPRequestHandler):
    simulator: Simulator

    def do_GET(self) -> None:  # noqa: N802
        if self.path == "/" or self.path.startswith("/?"):
            self._send(PAGE.encode("utf-8"), "text/html; charset=utf-8")
            return
        if self.path.startswith("/frame"):
            try:
                frame, metadata = self.simulator.frame()
                self.send_response(HTTPStatus.OK)
                self.send_header("Content-Type", "application/octet-stream")
                self.send_header("Content-Length", str(len(frame)))
                self.send_header("Cache-Control", "no-store")
                for name, value in metadata.items():
                    self.send_header(f"X-Garbox-{name.title()}", value)
                self.end_headers()
                self.wfile.write(frame)
            except (BrokenPipeError, RuntimeError) as error:
                self.send_error(HTTPStatus.SERVICE_UNAVAILABLE, str(error))
            return
        self.send_error(HTTPStatus.NOT_FOUND)

    def do_POST(self) -> None:  # noqa: N802
        if self.path != "/input":
            self.send_error(HTTPStatus.NOT_FOUND)
            return
        try:
            length = int(self.headers.get("Content-Length", "0"))
            if length <= 0 or length > 4096:
                raise ValueError("invalid request length")
            payload = json.loads(self.rfile.read(length))
            action = payload.get("action")
            if action == "rotate":
                value = int(payload.get("value", 0))
                if value not in (-1, 1):
                    raise ValueError("rotation must be -1 or 1")
                self.simulator.command(f"encoder {value}")
            elif action in ("press", "long", "release"):
                self.simulator.command(f"button {action}")
            else:
                raise ValueError("unknown control action")
            self._send(b"{}", "application/json")
        except (BrokenPipeError, RuntimeError, ValueError, json.JSONDecodeError) as error:
            self.send_error(HTTPStatus.BAD_REQUEST, str(error))

    def _send(self, body: bytes, content_type: str) -> None:
        self.send_response(HTTPStatus.OK)
        self.send_header("Content-Type", content_type)
        self.send_header("Content-Length", str(len(body)))
        self.send_header("Cache-Control", "no-store")
        self.end_headers()
        self.wfile.write(body)

    def log_message(self, _format: str, *_args: object) -> None:
        pass


def platformio_command() -> str:
    found = shutil.which("pio") or shutil.which("platformio")
    if found:
        return found
    fallback = Path.home() / ".platformio" / "penv" / "bin" / "platformio"
    if fallback.is_file():
        return str(fallback)
    raise RuntimeError("PlatformIO was not found; install it or build with the IDE first")


def build() -> None:
    print("Building the real LVGL simulator…")
    subprocess.run([platformio_command(), "run", "-e", "ui-simulator"], cwd=ROOT, check=True)


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--host", default="127.0.0.1", help="local bind address")
    parser.add_argument("--port", type=int, default=8765, help="local HTTP port")
    parser.add_argument("--no-build", action="store_true", help="reuse the existing simulator binary")
    parser.add_argument("--no-browser", action="store_true", help="do not open the browser automatically")
    args = parser.parse_args()

    if not args.no_build:
        build()
    if not PROGRAM.is_file():
        raise RuntimeError(f"simulator executable not found: {PROGRAM}")

    simulator = Simulator(PROGRAM)
    ticker: SimulationTicker | None = None
    server: ThreadingHTTPServer | None = None
    try:
        Handler.simulator = simulator
        ticker = SimulationTicker(simulator)
        server = ThreadingHTTPServer((args.host, args.port), Handler)
        server.daemon_threads = True
        address = f"http://{args.host}:{server.server_port}/"
        ticker.start()
        print(f"Garbox UI simulator: {address}")
        print("Press Ctrl+C here to stop it.")
        if not args.no_browser:
            webbrowser.open(address)
        server.serve_forever(poll_interval=0.25)
    except KeyboardInterrupt:
        print("\nStopping simulator…")
    finally:
        if server is not None:
            server.server_close()
        if ticker is not None:
            ticker.stop()
        simulator.close()
    return 0


if __name__ == "__main__":
    try:
        raise SystemExit(main())
    except (OSError, RuntimeError, subprocess.CalledProcessError) as error:
        print(f"error: {error}", file=sys.stderr)
        raise SystemExit(1)

# Finishing plan

## Definition of “finished”

For this project, “finished” should mean more than “the display looks right and the heater switches.” A dependable fermentation box should:

- always boot and fail into a heater-off state;
- never depend on the UI task to enforce thermal safety;
- regulate to a target with characterized overshoot and settling time;
- detect missing, stale, implausible, or stuck sensor input;
- detect fan and heater electrical faults that the available hardware can observe;
- recover predictably from sensor communication failures;
- expose understandable status/faults to the operator;
- survive long runs, timer wraparound, UI switching, and repeated fault recovery;
- be reproducibly buildable and testable from a fresh clone.

## Phase 0 — Freeze and baseline

Do this before more framework or UI work.

- Commit or intentionally discard the current uncommitted hardware/input/display/SHT31 work after testing it on the actual board.
- Record the exact ESP32-S3 module, flash/PSRAM variant, display module, heater supply, switching hardware, fan, SHT31 placement, and power topology.
- Add a real root README with setup, generator, build, test, upload, pinout, and operating instructions.
- Add a Python dependency manifest and a single generator command.
- Decide whether `res/` source assets should be tracked. They should be tracked if a fresh clone is expected to regenerate `src/bin`.
- Add CI or a local verification script that runs generation in a temporary tree and checks for a clean diff.

Exit gate: a fresh checkout can reproduce generated output and build the firmware without relying on an undocumented local virtual environment.

## Phase 1 — Remove immediate hazards and correctness traps

Recommended order:

1. Boot with regulation disabled and verify the physical heater output remains off through reset and initialization.
2. Make safety states override the user fan setting.
3. Remove the long-hold `TriggerExit("Testing")` path.
4. Register the fan tachometer ISR exactly once.
5. Strengthen control input validity: running, first sample, freshness, finite values, and plausible bounds.
6. Add an absolute temperature limit independent of the target.
7. Correct PID anti-windup and guard invalid/non-positive `dt`.
8. Use heater voltage/current to latch actionable faults where the circuitry supports it.
9. Make runtime dispatch fail safely if it cannot reach a stable state before releasing event storage.
10. Fix generated `next...()` accessors and regenerate.

Exit gate: deliberate tests of sensor removal, fan stall, user fan-off, target extremes, task failure, and over-temperature all leave the heater physically off and report a clear fault.

## Phase 2 — Establish a host-testable control core

Fix the missing header includes so the native environment compiles. Then expand tests in this order:

### Fermentation engine tests

- disabled → reset outputs;
- enable with no sample → ready/invalid and heater off;
- first valid sample → regulate;
- over-temperature entry and delayed exit;
- target change while over-temperature;
- fan-stall response;
- cooldown entry/exit;
- NaN, infinity, implausible temperature/humidity, and stale samples;
- PID saturation and recovery;
- target boundary handling.

Make time an injected input for these tests rather than relying on ESP32 globals.

### State/event/runtime tests

- current/next/last publication semantics;
- dirty coalescing;
- event value initialization;
- cascaded event/state routing;
- dispatch-depth exhaustion;
- queue and pool exhaustion;
- active/inactive behavior and screen routing;
- repeated request for the already-active component.

### Utility tests

- timer wraparound;
- software PWM at 0%, 100%, period changes, enable/disable, and rollover;
- FSM delayed transitions/cancellation/hold time;
- PID anti-windup;
- ring/vector/data-pool capacity and lifetime behavior;
- time-series all windows, buffer overrun, and sequence wrap;
- fan monitor callback/no-callback combinations.

### Generator tests

- invalid references fail with useful errors;
- duplicate definitions fail instead of silently overwriting;
- stable ordering independent of filesystem traversal;
- generated code compiles;
- generated sections preserve handwritten code;
- output is deterministic.

Exit gate: control and runtime behavior can be changed with fast native feedback, and tests cover every heater-enabling transition.

## Phase 3 — Add a dedicated safety supervisor

Keep safety separate from the normal fermentation policy. A small supervisor should observe inputs and commanded/measured outputs and issue the final heater permission.

Suggested latched faults:

- sensor missing or stale;
- temperature outside plausible range;
- absolute over-temperature;
- rate-of-rise inconsistent with normal operation;
- fan stalled when circulation is required;
- heater current absent while commanded on;
- heater voltage/current present while commanded off;
- control loop or main tick deadline missed;
- repeated I2C recovery beyond a threshold.

Suggested reset policy:

- transient communication faults may auto-recover after a stable validation period;
- over-temperature and unexpected heater current should require explicit operator acknowledgement or power cycle;
- the output GPIO’s safe physical level must be verified during reset, bootloader, and firmware startup;
- use an independent thermal fuse/cutoff where heater power warrants it.

Exit gate: every identified single fault either remains safe or is mitigated independently of the normal PID loop.

## Phase 4 — Tune against simulation and hardware

First make the Python simulation reproducible:

- add an environment file;
- move representative notebook setups into scriptable scenarios;
- share or explicitly translate PID behavior between Python and C++;
- save plots and numeric metrics rather than relying only on interactive display.

Then identify the physical system:

1. Log ambient, air, product surrogate, heater voltage/current, fan RPM, and command values.
2. Run safe open-loop power steps.
3. Fit thermal capacities/resistances and transport delay.
4. Choose control period and output constraints.
5. Tune in simulation.
6. Re-test on hardware at minimum/nominal/maximum load and ambient conditions.

Track at least:

- rise time;
- overshoot;
- settling band/time;
- steady-state error;
- heater switching rate;
- response to door opening and ambient change;
- response to sensor and fan faults.

Exit gate: control performance is documented over the intended temperature/load range, not just at one bench condition.

## Phase 5 — Finish the product behavior

### User experience

- Implement or remove the Calibration, About, and Debug menu actions.
- Define what calibration means: sensor offset, heater characterization, fan characterization, or all three.
- Implement simple-screen fan status.
- Re-enable or remove display brightness commands.
- Use one target-temperature range everywhere.
- Correct history labels and clearly mark unavailable/stale data.
- Expose fault reason, whether it is latched, and how to clear it.
- Decide whether short-press power toggling is too easy to activate accidentally.

### Persistence

Decide what survives reboot:

- target temperature;
- fan/circulation preference;
- calibration data;
- last fault;
- run history;
- active run plan.

Do not persist “heater on” as a simple boolean. Restore only through a deliberate, validated resume policy.

### History

- Correct 64-bit timing/averaging and iterator overrun behavior.
- Decide whether hierarchical downsampling is preferable to sampling all eight windows at 30 Hz.
- Decide whether four-day history must survive reset.
- Add timestamps or boot/session markers if persisted.

Exit gate: every displayed control works, status is truthful, ranges are consistent, and reboot behavior is explicit.

## Phase 6 — Hardware and endurance validation

Run a written matrix rather than ad hoc bench tests:

| Test | Expected result |
|---|---|
| Cold boot / warm reset / brownout | Heater remains off until explicit validated enable. |
| SHT31 unplugged, bus stuck, corrupted sample | Heater off; clear sensor fault; bounded recovery attempts. |
| Fan disconnected or blocked | Stall detected; heater policy safe; alarm and fault visible. |
| Heater disconnected | No-current fault when commanded on. |
| Heater switch stuck on (safely simulated) | Independent cutoff/fault; software command alone is not trusted. |
| Encoder/button noise and long hold | No crash or unintended dangerous setting change. |
| Rapid target/power changes | No stale commands, queue corruption, or unsafe transient. |
| Screen cycling during long run | No heap erosion, chart corruption, or missed control deadlines. |
| 72-hour and multi-day soak | Stable memory, timing, sensor recovery, and history. |
| Timer/sequence wrap tests | No false expiry, huge delta, or chart replay. |

Exit gate: multi-day operation and injected faults meet the finished definition.

## Suggested first implementation milestone

A focused first milestone could be called **Safe Core**:

- native build repaired;
- regulation defaults off;
- explicit shared sensor-validity/freshness rule;
- safety fan precedence fixed;
- long-hold crash removed;
- duplicate ISR registration removed;
- PID anti-windup fixed;
- absolute high-temperature cutoff added;
- fermentation engine covered by host tests;
- state `next` getter and dispatch lifetime defects covered by tests;
- heater electrical readings at least checked against simple on/off expectations.

That milestone deliberately postpones menu polish and broader framework work. It converts the current impressive prototype into a trustworthy base for finishing the box.


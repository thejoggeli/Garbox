# UI flow and controls

## Purpose

The UI is designed around the single rotary encoder and push button fitted to the Garbox PCB. Home keeps normal fermentation operation immediate; Main combines trends with the settings that are useful during a run; the three advanced screens keep bring-up and diagnostics available without cluttering the normal path.

This document describes the implementation verified in the desktop LVGL simulator on 2026-10-26. The simulator renders the same generated object trees, screen classes, fonts, theme, and application state/event contracts as the firmware. It does not validate the physical display bus, encoder direction, electrical polarity, sensor accuracy, or actuator behavior.

## Navigation map

```text
 Home ───────── hold ─────────► Main
  ▲                         turn/press: menu
  │                              │
  └──── hold or Home row ─────────┤
                                 │ open
                                 ▼
                    Diagnostics ─press─► Event Log
                         ▲                    │
                         └──── press ── State Log
                                 │
                     hold from any advanced screen
                                 │
                                 └──────────► Main
```

Main also has an explicit Home row. Diagnostics, Event Log, and State Log each have explicit Main-menu rows, so operators do not need to remember a hidden screen sequence.

## Physical-control contract

| Context | Rotate | Short press | Long press |
|---|---|---|---|
| Home | Adjust target by 0.1 °C | Start/stop fermentation | Open Main |
| Main, browsing | Move selection | Toggle binary value, enter edit mode, or open selected screen | Return Home |
| Main, editing | Change the selected value | Accept value and leave edit mode | Return Home |
| Diagnostics | Scroll the report vertically | Open Event Log | Return Main |
| Event Log | Previous/next advanced view | Open State Log | Return Main |
| State Log | Previous/next advanced view | Open Diagnostics | Return Main |

Every screen reserves the bottom 20 pixels for the same command bar. The action
for a short press is always shown on the left and the long-press destination is
always shown on the right. Screen content and transient status text never share
that area.

The Main header explicitly reports `BROWSE` or `EDIT`. In browse mode the selected
row and rotation arrows are blue. In edit mode only the value and arrows are
amber, and the footer changes to `PRESS: DONE`. Power and fan mode are binary,
so pressing either row toggles it immediately without entering edit mode.

## Home

Home answers the questions that matter during normal use, in visual priority order:

1. What is the chamber temperature?
2. Is regulation off, starting, heating, holding, cooling, or faulted?
3. What target am I controlling toward?
4. What are humidity, heater duty, and fan speed doing?

The large chamber value and outer measured arc dominate the screen. The thinner amber target arc provides a stable reference, while the measured arc changes color according to error relative to the target. The text below the value reports the signed error. A separate outlined target card places blue left/right arrows directly around the setpoint, mirroring encoder rotation without an additional instruction label.

The header is a single full-width control-state banner. Normal sensor health and
automatic fan policy are intentionally omitted: neither requires action, while
sensor failures and exceptional cooling states appear directly in the banner and
the affected measurement cards.

The status pill uses explicit words rather than color alone:

| State | Home label | Meaning |
|---|---|---|
| Power off | `CONTROL OFF` | Regulation is disabled. |
| Reset/initial transition | `STARTING` | Control is entering a usable state. |
| Ready | `CONTROL READY` | Enabled but waiting to regulate. |
| Regulating with heat | `HEATING` | Heater duty is non-zero. |
| Regulating without heat | `HOLDING` | Running at zero heater duty. |
| Cooldown | `COOLING` | Heater off, safety cooling active. |
| Over-temperature | `OVER TEMPERATURE` | Heater off, safety cooling active. |
| Invalid input | `SENSOR FAULT` | Input fault; heater off and safety fan requested. |

If the sensor is not ready, measured temperature and humidity show placeholders rather than default data.

## Main: trends and controls

The left side preserves the original project's strongest advanced feature: multi-window time series. It plots measured temperature and target together, plots heater output separately, shows current values, and labels the selected interval. Available history windows are 1, 4, and 16 minutes; 1, 4, and 12 hours; and 1 and 4 days.

The right side contains current system state, a six-row scrolling menu, and compact fan, humidity, electrical-power, and signed temperature-error tiles. The error tile replaces the target tile because target is already represented by both the graph and menu. The shared command bar provides the persistent Home hint.

### Settings and actions

| Row | Range/options | Effect |
|---|---|---|
| Power | Off / On | Disables or enables regulation. |
| Target | 15.0–50.0 °C, 0.1 °C steps | Changes the fermentation target. |
| Fan mode | Auto / Manual | Chooses engine-requested or user-requested normal fan output. |
| Fan speed | 0–100%, 5% steps | Sets the manual fan command. The value is retained while Auto is selected. |
| Brightness | 0–100%, 5% steps | Sends the real display-brightness command. |
| History | 1 minute–4 days | Selects the chart aggregation window. |
| Plot min | 10 or 20 °C | Selects the lowest labeled temperature-grid value. |
| Plot max | 40, 50, or 60 °C | Selects the highest labeled temperature-grid value. |
| Diagnostics | Action | Opens live engineering diagnostics. |
| Event log | Action | Opens the routed-event log. |
| State log | Action | Opens the published-state log. |
| Home | Action | Returns to Home. |

Manual fan control deliberately does not mean “override safety.” Invalid sensor input, over-temperature, and cooldown take precedence and command the engine's safety fan output. There is no manual heater control in the UI; introducing one would bypass the normal thermal policy and is outside this design.

## Advanced screens

Diagnostics retains the complete engineering data set but groups it into four
full-width cards: System, Climate, Outputs, and Memory. It shows time and the raw
microsecond clock, active behavior/screen, event and state counts, heap data,
display status, fermentation state/target, SHT31 status and sample, fan
state/RPM, heater state/electrical measurements, and PWM duty/period. A fill bar
at the bottom of Outputs visualizes heater PWM cycle progress. The report is
intentionally taller than the display: rotating scrolls it vertically while the
header and command bar remain fixed, leaving room to add future diagnostics
without compressing the existing data.

Heater duty and period show one value when the applied and queued PWM settings
match. During the interval between a control update and the next PWM-cycle
boundary, Diagnostics instead displays `applied -> queued`. The desktop plant
currently applies its idealized heater output immediately, so it normally shows
the single-value form.

Event Log is a rolling view of routed event ID, sender, and event type. State Log is a rolling view of state-publication sequence and state type. Both retain a marker on the newest entry. Short press moves from Diagnostics to Event Log and onward to State Log; long press returns to Main. Rotation navigates between the two compact log views, while Diagnostics reserves it for scrolling.

## Desktop verification

Build and run the automated flow from the repository root:

```bash
/home/michael/.platformio/penv/bin/platformio run -e ui-simulator
python3 tools/verify_ui_flow.py --capture-dir /tmp/garbox-ui-flow
```

The verifier drives typed `EncoderStep` and `Button` events through the real screens. It checks:

- Home target adjustment from 32.0 to 32.5 °C and start/stop;
- Main power off/on editing;
- Main target editing and restoration;
- switching fan mode to Manual, selecting 35%, and observing a 35% behavior output;
- direct entry to Diagnostics and rotation-driven diagnostic scrolling;
- the complete short-press advanced-screen loop and log-view navigation;
- return through Main to Home without losing power or target state;
- a correctly sized framebuffer for every captured screen.

The sanitizer-backed native suite separately verifies normal regulation, sensor/fan failure, over-temperature, cooldown, manual fan output while regulation is off, and safety cooling overriding a manual-off preference.

For interactive use:

```bash
python3 tools/ui_simulator.py
```

The browser page maps arrows, mouse wheel, or left/right keys to rotation. Click/Space/Enter provides a short press; holding for 0.7 seconds provides a long press.

## Remaining hardware checks

Before treating the UI as hardware-complete, verify on the PCB:

- physical clockwise direction matches increasing target/menu direction;
- one encoder detent produces one logical step;
- short versus long press timing feels deliberate and does not double-trigger;
- every screen is oriented and colored identically to the simulator;
- the smallest 8 px hints remain readable on the actual panel and enclosure;
- brightness reaches a useful minimum/maximum and zero does not create a recovery trap;
- fan speed, heater duty, RPM, electrical power, and fault labels match measured hardware;
- long-duration history windows remain numerically and temporally correct.

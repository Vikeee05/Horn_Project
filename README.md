1. Overview

Excessive and indiscriminate horn honking is a major contributor to urban noise pollution. This project aims to introduce a behavioral and economic disincentive by treating horn usage as a metered, prepaid resource — just like mobile airtime or data.

Every time the driver presses the horn, the device measures the press duration and deducts credits from a prepaid balance. The balance is maintained both locally on the device (for offline/real-time deduction) and on a cloud server (for account management, recharge, and reporting). The device connects to the cloud via GSM, so it works independent of the vehicle's own connectivity or a paired smartphone.

The end hardware is designed to be extremely compact — approximately the size of a wristwatch dial — so it can be discreetly mounted near the horn switch/relay wiring inside the dashboard.

2. Core Concept: The Credit System
Horn press duration	Credits consumed
250 ms (1 fragment)	1 credit
500 ms	2 credits
1000 ms (1 second)	4 credits
N ms	ceil(N / 250) credits

Rules of thumb / open design questions to finalize:

Fragment quantization: every started 250 ms fragment counts as a full credit (i.e., a 260 ms press = 2 credits), similar to how telecom pulse billing rounds up. This should be explicitly confirmed and documented in firmware comments, since it directly affects billing fairness.
Continuous vs. re-triggered press: a press-hold-release counts continuously; rapid re-presses (tap-tap-tap) each start a new timing window.
Minimum debounce: presses shorter than a configurable debounce threshold (e.g., 20–30 ms) should be filtered out as electrical noise/contact bounce, not billed.
3. System Architecture
                ┌─────────────────────────────┐
                │        Cloud Platform        │
                │  (Account, Wallet, Recharge,  │
                │   Usage History, Billing API) │
                └───────────────┬───────────────┘
                                │  GSM (HTTPS/MQTT over GPRS/LTE-M)
                                │
                ┌───────────────▼───────────────┐
                │         HonkMeter Device        │
                │ ┌───────────┐   ┌─────────────┐ │
                │ │   MCU     │───│ GSM Module  │ │
                │ │ (control, │   │ (SIM-based) │ │
                │ │ credit    │   └─────────────┘ │
                │ │ logic)    │                    │
                │ │           │   ┌─────────────┐ │
                │ │           │───│ Local Store │ │
                │ │           │   │ (Flash/EEPROM)│
                │ └─────┬─────┘   └─────────────┘ │
                │       │                          │
                │ ┌─────▼─────┐   ┌──────────────┐│
                │ │ Horn Press │   │ Power Mgmt/   ││
                │ │ Detection  │   │ Battery +     ││
                │ │ (GPIO/     │   │ Charging      ││
                │ │  ADC/Opto) │   │ Circuit       ││
                │ └─────┬──────┘   └──────┬───────┘│
                └───────┼──────────────────┼────────┘
                        │                  │
                 Horn switch/relay    Vehicle 12V rail
                   signal line          (ignition-on)
3.1 Device-side responsibilities
Detect horn press start/end and measure duration.
Maintain an authoritative local credit balance (works even with no GSM signal).
Deduct credits locally in real time as the horn is used.
Periodically (or event-driven) sync local consumption and balance with the cloud.
Fetch/refresh balance from cloud after a recharge event.
Manage power source switching between vehicle supply and internal battery.
3.2 Cloud-side responsibilities
Maintain the source-of-truth wallet/credit balance per device/user.
Expose a recharge API (payment gateway integration, top-up like a mobile recharge).
Accept usage/consumption reports from the device and reconcile balance.
Provide a dashboard/app for the driver to view balance, usage history, and recharge.
Handle conflict resolution if local and cloud balances diverge (e.g., device was offline for a while).
4. Hardware Design
4.1 Form Factor Constraint
Target enclosure: ~40 mm diameter, wristwatch-dial-sized, circular PCB.
This heavily constrains component choice — favors a single compact SoC (MCU + BLE/GSM co-processor where possible) and a small-form-factor GSM module (e.g., Cat-M1/NB-IoT or 2G/GPRS module in an LCC/LGA package) plus a slim LiPo pouch cell.
4.2 Major Components (proposed — to be finalized during hardware selection)
Block	Candidate Component(s)	Notes
MCU	STM32L4/STM32U5 (ultra-low-power) or ESP32-C3/S3	Needs low-power sleep modes since device must run off battery for extended periods with the car off
GSM/Cellular	SIM7000/SIM7080 (NB-IoT/Cat-M1) or SIM800/SIM868 (2G, cheaper, wider legacy coverage)	Cat-M1/NB-IoT preferred for lower power draw and smaller antenna, subject to network availability
Horn press detection	Opto-isolated GPIO input or voltage divider + comparator on horn relay/switch line	Must tolerate automotive 12V/24V transients; opto-isolation recommended for safety and noise immunity
Local storage	Onboard MCU flash + external I2C/SPI EEPROM or FRAM	FRAM preferred for high write-endurance since credits update very frequently
Battery	Single-cell LiPo (e.g., 150–300 mAh, coin/pouch form factor)	Sized to fit the watch-dial enclosure
Charging	Linear Li-ion charger IC (e.g., TP4056/BQ24074 or automotive-input variant) fed from vehicle 12V rail via a buck pre-regulator	Charges only while ignition/accessory line is powered (car "on")
Voltage protection	TVS diodes, reverse-polarity protection, load-dump protection	Automotive 12V rail is electrically noisy (load dump, transients) — essential for reliability
RTC	Onboard RTC with backup coin cell or supercap (if needed)	For accurate timestamping of usage events during GSM outage
4.3 Horn Press Detection
The device should be wired in parallel with the horn switch/relay control line (not in series with the horn's high-current path), so it senses the press signal without carrying horn current.
Recommended: an opto-coupler or automotive-grade digital isolator feeding a GPIO input, with a pull configuration set so the MCU sees a clean digital HIGH/LOW transition on press/release.
Firmware performs hardware debounce (RC filter) + software debounce before starting/stopping the credit timer.
4.4 Power Architecture
Vehicle 12V (ignition/accessory) ──► Reverse-polarity + surge protection ──► Buck regulator ──► Battery charger IC ──► Li-ion/LiPo cell ──► System LDO/Buck ──► MCU + GSM
                                                                                      ▲
                                                                                      │
                                                              Car OFF: charger idle, battery powers system directly
Car ON: vehicle rail powers the system directly and charges the battery.
Car OFF: system runs entirely off the battery — horn presses can still be metered and deducted locally even with the engine off (e.g., horn used as anti-theft alarm or while parked); sync to cloud resumes once GSM is available and/or car is on again.
Firmware should implement low-power/sleep states when idle to maximize battery life between charge cycles, waking on a GPIO interrupt (horn press) or a periodic timer (housekeeping sync).
5. Firmware / Software Flow
5.1 Startup
Boot MCU, initialize peripherals (GPIO, storage, GSM module).
Read last known local credit balance from non-volatile storage.
Attempt GSM network registration; if successful, sync balance with cloud (reconcile any pending offline usage first).
Enter low-power idle/listen state.
5.2 Horn Press Event
GPIO interrupt on horn-line transition (press).
Start high-resolution timer.
On release, compute elapsed duration.
Compute credits = ceil(duration_ms / 250).
Deduct credits from local balance (if insufficient balance, define product behavior — e.g., allow with negative balance and flag, or mute horn feedback/alert, per business rules to be decided).
Log the usage event (timestamp, duration, credits) to local storage.
Queue event for cloud sync.
5.3 Cloud Sync
Sync can be event-driven (immediately after each press, if GSM is available) and/or batched (periodic interval, e.g., every N minutes, or every N credits consumed) to conserve GSM data/power.
On each sync: upload queued usage events, and pull authoritative balance from server (to reflect recharges made via app/web).
Implement retry with backoff and local queue persistence for offline periods.
Define a clear reconciliation policy: local balance is authoritative for real-time deduction (so the horn always works immediately), while server balance is authoritative for the account — periodic reconciliation corrects drift.
5.4 Recharge Flow
Driver recharges via a mobile app / web portal (cloud-side, outside device scope).
Cloud updates the user's wallet balance.
Cloud pushes (or device pulls, if using MQTT/pub-sub or a periodic poll) the updated balance to the device.
Device updates its local balance accordingly.
6. Data Model (Cloud, indicative)

User/Account

user_id, name, vehicle_id, device_id, wallet_balance_credits, created_at

Device

device_id, sim_iccid, last_seen_at, last_synced_balance, firmware_version

Usage Event

event_id, device_id, timestamp, duration_ms, credits_consumed, synced (bool)

Recharge Transaction

transaction_id, user_id, amount, credits_added, payment_ref, timestamp
7. Communication Protocol (Device ↔ Cloud)
Transport: GSM (GPRS/2G, Cat-M1, or NB-IoT depending on module chosen), running MQTT (lightweight, good for periodic small payloads + push balance updates) or HTTPS REST (simpler to implement, slightly heavier per-request overhead).
Suggested topics/endpoints (if MQTT):
device/{device_id}/usage — device publishes usage events
device/{device_id}/balance — server publishes updated balance
device/{device_id}/heartbeat — periodic device health/status
Security: TLS for transport, per-device authentication (e.g., unique token/certificate per device), signed/checksummed payloads to prevent tampering with credit reports.
8. Open Design Decisions / To Be Finalized
 Exact MCU and GSM module selection (2G/GPRS vs. NB-IoT/Cat-M1) based on target region network coverage and power budget.
 Battery capacity vs. expected standby/active life (needs load analysis: GSM module current draw is the dominant factor).
 Behavior when local credit balance is zero (block horn feedback? allow with warning? this may have safety implications — a horn is a safety device, so blocking it outright needs careful legal/safety review).
 Enclosure design and mounting method inside the dashboard/steering column area.
 Data sync frequency vs. GSM data cost trade-off.
 Tamper detection (e.g., device removed/disconnected from horn line).
9. Repository Structure (proposed)
/firmware        - MCU firmware source (credit logic, horn detection, power management, GSM stack)
/hardware        - Schematics, PCB layout, BOM (KiCad/Altium project files)
/cloud           - Backend services (account, wallet, recharge, sync API)
/app             - Mobile/web app for driver (balance view, recharge, usage history)
/docs            - Design documents, protocol specs, test reports
README.md        - This file
10. Status

🚧 Concept / Early Design Phase — architecture and hardware component selection in progress.

11. License

(To be decided by project owner)
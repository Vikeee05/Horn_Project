# Horn Device Software Architecture

## 1. System overview

The horn billing device records horn activation time and converts it into prepaid usage credits.

## 2. Key behavior

- 1 credit = 250 ms
- 1 second = 4 credits
- Local balance is decreased when horn is used
- A GSM link synchronizes balance updates with the server
- If the car is OFF, the device is battery powered
- If the car is ON, the charging circuit keeps the battery charged

## 3. Main software modules

### Horn Input Acquisition

Reads the horn signal line and measures the active duration.

### Credit Accounting Logic

Converts duration into credits and validates whether the user has enough balance.

### Cloud Synchronization

Pushes local usage and receives balance updates or recharge confirmations.

### Power and Battery Logic

Tracks ignition state, charging condition, and battery health.

## 4. Example logic

For a horn press of 1250 ms:

- 1250 / 250 = 5 credits

For a horn press of 800 ms:

- 800 / 250 = 3 credits remaining after floor division
- Additional partial credit can be tracked if required by design

## 5. Development notes

The current software is a starter project and is designed to be adapted to the target STM32 controller and modem stack.

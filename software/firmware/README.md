# Horn Billing Firmware

This firmware project is a simple RTOS-based starter for an STM32 horn metering controller.

## Features included in the sample

- Horn press detection using GPIO input
- Duration measurement in milliseconds
- Credit conversion using 250 ms per credit
- Local credit balance tracking
- Cloud sync task abstraction using GSM module interface
- Power and ignition state monitoring
- FreeRTOS task structure

## Assumptions

- MCU: STM32 series (for example STM32F4 or STM32G0)
- RTOS: FreeRTOS
- Communication: GSM modem over UART
- Inputs: horn switch signal and ignition detection signal

## Task model

- horn_input_task
- credit_logic_task
- gsm_sync_task
- power_monitor_task

## Typical design flow

1. Detect horn line transition
2. Measure active time
3. Convert to credit units
4. Consume local balance
5. Push update to cloud server
6. Re-sync local balance when recharge is received

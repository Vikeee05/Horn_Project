CC = arm-none-eabi-gcc
CFLAGS = -Wall -Wextra -std=c11 -mcpu=cortex-m4 -mthumb -Iinclude
MAP = horn_billing_firmware.map
LDFLAGS = --specs=nosys.specs -Tstm32f407.ld -Wl,--gc-sections -Wl,-Map=$(MAP)
SRC = src/main.c src/horn_credit_manager.c src/cloud_sync.c src/battery_monitor.c src/board.c src/startup_stm32f407xx.c
OBJ = $(SRC:.c=.o)
TARGET = horn_billing_firmware.elf

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(OBJ)
	arm-none-eabi-objcopy -O binary $@ horn_billing_firmware.bin

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET) horn_billing_firmware.bin $(MAP)

.PHONY: all clean

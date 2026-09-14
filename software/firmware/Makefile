CC = arm-none-eabi-gcc
CFLAGS = -Wall -Wextra -std=c11 -mcpu=cortex-m4 -mthumb -Iinclude
BUILD_DIR = Build
TARGET = $(BUILD_DIR)/horn_billing_firmware.elf
BIN = $(BUILD_DIR)/horn_billing_firmware.bin
MAP = $(BUILD_DIR)/horn_billing_firmware.map
SRC = src/main.c src/horn_credit_manager.c src/cloud_sync.c src/battery_monitor.c src/board.c src/startup_stm32f407xx.c
OBJ = $(SRC:src/%.c=$(BUILD_DIR)/%.o)

ifeq ($(OS),Windows_NT)
MKDIR = if not exist "$(BUILD_DIR)" mkdir "$(BUILD_DIR)"
CLEAN = if exist "$(BUILD_DIR)" (for /f %%F in ('dir /b /a-d "$(BUILD_DIR)" 2^>nul ^| find /c /v ""') do @echo Cleaning %%F generated files: & dir /b /a-d "$(BUILD_DIR)" & rmdir /S /Q "$(BUILD_DIR)") else (echo Nothing to clean.)
MEMORY_REPORT = powershell -NoProfile -ExecutionPolicy Bypass -File tools/memory_report.ps1 -Elf "$(TARGET)"
REPORT = @echo Build completed successfully. & echo Binary size: & for %%F in ("$(BIN)") do @echo   %%~zF bytes & echo ELF memory usage: & arm-none-eabi-size $(TARGET) & $(MEMORY_REPORT) & echo ELF section sizes: & arm-none-eabi-size -A $(TARGET) & echo Memory map: $(MAP)
else
MKDIR = mkdir -p $(BUILD_DIR)
CLEAN = if test -d "$(BUILD_DIR)"; then echo "Cleaning $$(find "$(BUILD_DIR)" -type f | wc -l) generated files:"; find "$(BUILD_DIR)" -type f -print; rm -rf "$(BUILD_DIR)"; else echo "Nothing to clean."; fi
MEMORY_REPORT = arm-none-eabi-size $(TARGET) | awk 'NR == 2 { text = $$1; data = $$2; bss = $$3; flash = text + data; ram = data + bss; printf "FLASH: total 1048576 bytes, used %d bytes, available %d bytes (%.2f%% used)\n", flash, 1048576 - flash, 100 * flash / 1048576; printf "RAM:   total 131072 bytes, used %d bytes, available %d bytes (%.2f%% used)\n", ram, 131072 - ram, 100 * ram / 131072 }'
REPORT = @echo "Build completed successfully."; echo "Binary size: $$(wc -c < "$(BIN)") bytes"; echo "ELF memory usage:"; arm-none-eabi-size $(TARGET); $(MEMORY_REPORT); echo "ELF section sizes:"; arm-none-eabi-size -A $(TARGET); echo "Memory map: $(MAP)"
endif

LDFLAGS = --specs=nosys.specs -Tstm32f407.ld -Wl,--gc-sections -Wl,-Map=$(MAP)

help:
	@echo Available build options:
	@echo   make project  Build the firmware into the Build directory
	@echo   make clean    Remove generated build files

project: $(TARGET)
	$(REPORT)

$(BUILD_DIR):
	$(MKDIR)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(OBJ)
	arm-none-eabi-objcopy -O binary $@ $(BIN)

$(BUILD_DIR)/%.o: src/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	-@$(CLEAN)

.PHONY: help project clean

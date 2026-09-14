cmake_minimum_required(VERSION 3.16)

project(horn_billing_firmware C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -std=c11 -mcpu=cortex-m4 -mthumb")

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_executable(horn_billing_firmware
    src/main.c
    src/horn_credit_manager.c
    src/cloud_sync.c
    src/battery_monitor.c
    src/board.c
    src/startup_stm32f407xx.c
)

target_compile_definitions(horn_billing_firmware PRIVATE
    USE_FREERTOS=1
)

target_link_options(horn_billing_firmware PRIVATE
    --specs=nosys.specs
    -T${CMAKE_CURRENT_SOURCE_DIR}/stm32f407.ld
    -Wl,--gc-sections
    -Wl,-Map=${CMAKE_CURRENT_BINARY_DIR}/horn_billing_firmware.map
)

set_property(TARGET horn_billing_firmware PROPERTY LINK_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/stm32f407.ld
)

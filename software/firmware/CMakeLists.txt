cmake_minimum_required(VERSION 3.16)

project(horn_billing_firmware C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_executable(horn_billing_firmware
    src/main.c
    src/horn_credit_manager.c
    src/cloud_sync.c
    src/battery_monitor.c
)

target_compile_definitions(horn_billing_firmware PRIVATE
    USE_FREERTOS=1
)

# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)

# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.1.1)
set(toolchainVersion 14_2_Rel1)
set(picotoolVersion 2.1.1)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
set(PICO_BOARD pico_w CACHE STRING "Board type")

# Set any variables required for importing libraries
SET(FREERTOS_KERNEL_PATH ${CMAKE_CURRENT_SOURCE_DIR}/lib/FreeRTOS-Kernel)

# Import those libraries
SET(ProjectName fly_pong)
# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)
include(${FREERTOS_KERNEL_PATH}/portable/ThirdParty/GCC/RP2040/FreeRTOS_Kernel_import.cmake)

project(${ProjectName} C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add executable. Default name is the project name, version 0.1

add_executable(${ProjectName}
    src/main.c
    #src/iot_security_lab.c 
    src/led_matrix.c
    src/mqtt_comm.c
    src/wifi_conn.c
    src/xor_cipher.c
    src/led_rgb.c
    src/button.c
    src/buzzer.c
)

pico_set_program_name(${ProjectName} "fly_pong")
pico_set_program_version(${ProjectName} "0.1")

pico_generate_pio_header(${ProjectName} ${CMAKE_CURRENT_LIST_DIR}/ws2818b.pio)

# Modify the below lines to enable/disable output over UART/USB
pico_enable_stdio_uart(${ProjectName} 0)
pico_enable_stdio_usb(${ProjectName} 1)

# Add the standard library to the build
target_link_libraries(${ProjectName}
        # Biblioteca padrão do Pico SDK, que fornece funções básicas para o RP2040 (GPIO, temporizadores, UART, etc.).
        pico_stdlib
        pico_cyw43_arch_lwip_threadsafe_background
        pico_lwip_mqtt
        pico_lwip
        pico_cyw43_driver
        FreeRTOS-Kernel-Heap4
        hardware_pwm
        hardware_pio
        hardware_clocks
        hardware_adc
        # pico_time
        # pico_unique_id
)

# Add the standard include files to the build
target_include_directories(${ProjectName} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/include
)

# Add any user requested libraries
target_link_libraries(${ProjectName} 
        
        )

pico_add_extra_outputs(${ProjectName})


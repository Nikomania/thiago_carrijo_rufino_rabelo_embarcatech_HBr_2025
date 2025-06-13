add_executable(${ProjectName}
    main.c
    buzzer.c
    led_rgb.c
    button.c
)

pico_set_program_name(${ProjectName} "free_rtos_led_button_buzzer")
pico_set_program_version(${ProjectName} "0.1")

pico_enable_stdio_uart(${ProjectName} 0)
pico_enable_stdio_usb(${ProjectName} 1)

target_link_libraries(${ProjectName} 
    pico_stdlib 
    FreeRTOS-Kernel-Heap4
    hardware_pwm
)

target_include_directories(${ProjectName} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/include
    ${CMAKE_CURRENT_LIST_DIR}
)


pico_add_extra_outputs(${ProjectName})

cmake_minimum_required(VERSION 3.13)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

project(test_adc C CXX)

enable_testing()

# Cria executável de testes
add_executable(test_adc
    test_adc.c
    ../src/temperature.c
    unity.c
)

# Inclui headers
target_include_directories(test_adc PRIVATE ${CMAKE_CURRENT_LIST_DIR}/../inc)

# Comando para rodar testes via CTest
add_test(NAME adc_tests COMMAND test_adc)

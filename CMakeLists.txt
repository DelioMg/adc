cmake_minimum_required(VERSION 3.13)

# Define o nome do projeto
project(adc C CXX ASM)

# Define o diretório do SDK do Raspberry Pi Pico
include($ENV{PICO_SDK_PATH}/pico_sdk_init.cmake)

# Inicializa o SDK do Pico
pico_sdk_init()

# Adiciona o executável principal
add_executable(adc adc.c)

# Aloca bibliotecas padrão do Pico
pico_enable_stdio_usb(adc 1)
pico_enable_stdio_uart(adc 1)

# Vincula bibliotecas necessárias
target_link_libraries(adc
    pico_stdlib
    hardware_adc
    hardware_pwm
    hardware_i2c
)

# Criação do arquivo UF2 para o Raspberry Pi Pico
pico_add_extra_outputs(adc)

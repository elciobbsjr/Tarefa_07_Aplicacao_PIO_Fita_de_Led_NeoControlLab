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
# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Initialise pico_sdk from installed location
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

# Define a placa
set(PICO_BOARD pico_w CACHE STRING "Board type")

# Importa SDK do Pico
include(pico_sdk_import.cmake)

project(NeoControlLab C CXX ASM)

# Inicializa o SDK
pico_sdk_init()

# Define os arquivos do projeto
add_executable(NeoControlLab 
    NeoControlLab.c 
    display_oled.c 
    testes_cores.c 
    LabNeoPixel/util.c 
    LabNeoPixel/neopixel_driver.c 
    LabNeoPixel/efeitos.c 
    efeito_curva_ar.c 
    numeros_neopixel.c 
    inc/ssd1306_i2c.c           # <- IMPLEMENTAÇÃO agora está em inc/
)

# Nome e versão do programa
pico_set_program_name(NeoControlLab "NeoControlLab")
pico_set_program_version(NeoControlLab "0.1")

# Geração de header para PIO
pico_generate_pio_header(NeoControlLab ${CMAKE_CURRENT_LIST_DIR}/LabNeoPixel/ws2818b.pio)

# UART e USB
pico_enable_stdio_uart(NeoControlLab 0)
pico_enable_stdio_usb(NeoControlLab 1)

# Bibliotecas necessárias
target_link_libraries(NeoControlLab 
    pico_stdlib 
    hardware_pio 
    hardware_clocks 
    hardware_adc 
    hardware_dma 
    hardware_pwm 
    hardware_i2c 
)

# Diretórios de include (para .h)
target_include_directories(NeoControlLab PRIVATE 
    ${CMAKE_CURRENT_LIST_DIR} 
    ${CMAKE_CURRENT_LIST_DIR}/LabNeoPixel
    ${CMAKE_CURRENT_LIST_DIR}/inc    # <- onde estão os headers ssd1306_xxx.h
)

# Geração de arquivos extras
pico_add_extra_outputs(NeoControlLab)

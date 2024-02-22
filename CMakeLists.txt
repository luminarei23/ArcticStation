set(PROJECT ArcticStation)

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)
set(PICO_BOARD "pico_w")

# initialize the SDK based on PICO_SDK_PATH
# note: this must happen before project()
include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

project(${PROJECT} C CXX ASM)

# initialize the Raspberry Pi Pico SDK
pico_sdk_init()

# Include directories
include_directories(source/types source/sensors source/wifi)

set(SOURCES 
source/ArcticStation.cpp
source/sensors/PMS7003.cpp
source/types/AirSensorStream.cpp
)

# Executables goes there
add_executable(${PROJECT} 
    ${SOURCES}
)

add_compile_options(
        -Wall
        -Werror
        -Wno-redefined
        )

target_include_directories(${PROJECT} PRIVATE ${CMAKE_CURRENT_LIST_DIR})

# Add pico_stdlib library which aggregates commonly used features
target_link_libraries(${PROJECT} PRIVATE
pico_cyw43_arch_lwip_threadsafe_background
pico_cyw43_arch_none
pico_stdlib
)

# create map/bin/hex/uf2 file in addition to ELF.
pico_add_extra_outputs(${PROJECT})
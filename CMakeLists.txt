set(PROJECT ArcticStation)

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(PICO_BOARD "pico_w")

# initialize the SDK based on PICO_SDK_PATH
# note: this must happen before project()
include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

project(${PROJECT} C CXX)

# initialize the Raspberry Pi Pico SDK
pico_sdk_init()

# Include directories
include_directories(source/types source/sensors)

set(SOURCES 
source/ArcticStation.cpp
source/sensors/PMS7003.cpp
source/types/AirSensorStream.cpp
)

# Executables goes there
add_executable(${PROJECT} 
    ${SOURCES}
)

target_include_directories(${PROJECT} PRIVATE ${CMAKE_CURRENT_LIST_DIR})

# Add pico_stdlib library which aggregates commonly used features
target_link_libraries(${PROJECT} PRIVATE
pico_stdlib
pico_cyw43_arch_none 

)

# create map/bin/hex/uf2 file in addition to ELF.
pico_add_extra_outputs(${PROJECT})

# pico_enable_stdio_usb(${PROJECT} 0)
# pico_enable_stdio_uart(${PROJECT} 1)
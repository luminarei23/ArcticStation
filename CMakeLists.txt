cmake_minimum_required(VERSION 3.13)

# initialize the SDK based on PICO_SDK_PATH
# note: this must happen before project()
include(pico_sdk_import.cmake)

project(ArcticStation)

# initialize the Raspberry Pi Pico SDK
pico_sdk_init()

# Include directories
include_directories(source/types source/sensors)

set(SOURCES 
source/ArcticStation.cpp
source/sensors/PMS7003.cpp
source/types/StreamWrapper.cpp
)

# Executables goes there
add_executable(ArcticStation
    ${SOURCES}
)

# Add pico_stdlib library which aggregates commonly used features
target_link_libraries(ArcticStation pico_stdlib pico_cyw43_arch_none)

# create map/bin/hex/uf2 file in addition to ELF.
pico_add_extra_outputs(ArcticStation)

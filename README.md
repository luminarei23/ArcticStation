# ArcticStation

RPI pico W Based station for measuring weather

Functionalities:

- cyw43 WiFi connectivity (working)
- C++ library for PMS7003 sensor

# Move files from WSL to Windows

cp -r /home/luminarei/projects/ArcticStation/build/\* /mnt/e/repositories/ArcticStation/build/

# Building and Flashing

First navigate to luminarei/projects/ArcticStation/build and then run following commands:

- export PICO_SDK_PATH=/home/luminarei/projects/pico-sdk
- cmake -DPICO_BOARD=pico_w ..
- make ArcticStation

Now build files has been saved in build directory. To flash program via picoprobe run:

- sudo openocd -f interface/cmsis-dap.cfg -f target/rp2040.cfg -c "adapter speed 5000" -c "program ArcticStation.elf verify reset exit"

# Debugging with GDB

Open new terminal and run openocd server:

- sudo openocd -f interface/cmsis-dap.cfg -f target/rp2040.cfg -c "adapter speed 5000"

in second terminal open GDB and start debugging:

- gdb-multiarch ArcticStation.elf
- target extended-remote localhost:3333
- monitor reset init
- continue

# Using minicom for debugging

- sudo minicom -D /dev/ttyACM0 -b 115200

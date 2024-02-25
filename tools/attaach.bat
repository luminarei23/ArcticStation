@echo off
rem USBIPD Attach Script for Picoprobe USB device

rem Define the WSL2 instance name
set WSL_INSTANCE=Ubuntu

rem Define the Picoprobe USB device's bus ID
set USB_BUSID=4-4

rem Run usbipd attach command to attach the USB device to WSL2
usbipd attach --wsl --busid %USB_BUSID%
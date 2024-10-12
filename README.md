# Pixie Gauntlet

A compact sequel of the [UF0](https://github.com/Amphicheiras/Wireless-MIDI-controller-DEMO) wireless MIDI controller, focused on circuit design, code development, and motion tracking.

Keywords: ESP32, C++, VS Code, PlatformIO, UART, I2C, rtpMIDI, WiFi, IMU, OOP, FreeRTOS*, HTML, HTTP

## HARDWARE:

For this project I decided to solder the hardware together to have permanent, stable connections. I started with a perf board and designed the possible layouts in order to get the most compact piece of hardware. The following is what I came at:

<img src="https://github.com/Amphicheiras/Pixie-Gauntlet/blob/main/media/PX_Board_TOP.jpg?raw=true" alt="PX_BOARD_TOP" width="400">
<img src="https://github.com/Amphicheiras/Pixie-Gauntlet/blob/main/media/PX_Board_BOTTOM.jpg?raw=true" alt="PX_BOARD_BOTTOM" width="400">
<img src="https://github.com/Amphicheiras/Pixie-Gauntlet/blob/main/media/PX_Board_FRONT.jpg?raw=true" alt="PX_BOARD_FRONT" width="400">
<img src="https://github.com/Amphicheiras/Pixie-Gauntlet/blob/main/media/PX_Board_BACK.jpg?raw=true" alt="PX_BOARD_BACK" width="400">

The corresponding PCB trace-out would look something like this:

<img src="https://github.com/Amphicheiras/Pixie-Gauntlet/blob/main/media/PX_BOARD_PCB.png?raw=true" alt="PX_BOARD_PCB" width="400">

The above PCB design is not the optimal, but for the hardware in my stock, it did the trick. The next hardware version will be an optimized printed PCB circuit for the UF0 project.

The ESP32 pinout for this project can be seen below:

<img src="https://github.com/Amphicheiras/Pixie-Gauntlet/blob/main/media/PX_ESP32_PINOUT.png?raw=true" alt="PX_ESP32_PINOUT" width="700">

## EMBEDDED SOFTWARE:

The Pixie project is a continuation of the UF0 project, aiming for better code practices and motion sensor algorithms rather than feature implementation. The code structure is now more explicit, with better hierarchy and OOP, still, under development.

The basic components of this project are the:
- Hardware
- Music
- Network
- OS (Operating System)

<img src="https://github.com/Amphicheiras/Pixie-Gauntlet/blob/main/media/PX_src_structure.png" alt="PX_SRC_STRUCTURE">

The hardware components drive, unsuprisingly, the hardware parts of the project.

<img src="https://github.com/Amphicheiras/Pixie-Gauntlet/blob/main/media/PX_src_hardware_components.png" alt="PX_SRC_HARDWARE">

The music components handle MIDI operations and musical theory (soon to come).

<img src="https://github.com/Amphicheiras/Pixie-Gauntlet/blob/main/media/PX_src_music_components.png" alt="PX_SRC_MUSIC">

The network components handle the WiFi connections and the  webserver functionality.

<img src="https://github.com/Amphicheiras/Pixie-Gauntlet/blob/main/media/PX_src_network_components.png" alt="PX_SRC_NETWORK">

The OS components are in charge of the operation of the whole system and hold third party algorithms.

<img src="https://github.com/Amphicheiras/Pixie-Gauntlet/blob/main/media/PX_src_OS_components.png" alt="PX_SRC_OS">

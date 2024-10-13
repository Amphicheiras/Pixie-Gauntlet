# Pixie Gauntlet

## Compact Motion-Tracking Wireless MIDI Controller

**!!! DEMO VIDEO ---> https://drive.google.com/file/d/1lhsPMKKbq0_hhStSr28U_LeOdEvPMwc7/view?usp=sharing <--- !!!**

<img src="https://github.com/Amphicheiras/Pixie-Gauntlet/blob/main/media/PX_PROMO.jpg" alt="PX_BOARD_TOP" width="400">

Externally powered by lithium batteries:

<img src="https://github.com/Amphicheiras/Pixie-Gauntlet/blob/main/media/PX_WHOLE.jpg" alt="PX_BOARD_TOP" width="400">

**Features**:

- 6-dimensional motion tracking (3x Euler Angles + 3x Linear Acceleration)
- MIDI over WiFi
- Control Webpage
  - Solo/Start/Stop Transmissions
  - View the 6D values
  - Save/Load Preset
- Li-ion power supply
- 3x LED Buttons + 3x Modes
  - By pressing all 3 buttons, you go to the next mode 
  - Mode 1:
    - Button 1(Red): Play/Stop MIDI Note
    - Button 2(Green): Activate/Deactivate MIDI Controls
    - Button 3(Blue): Custom MIDI On/Off parameter (Mapped from DAW)
  - Mode 2:
    - Button 1(Red): Enable/Disable Pitch transmission
    - Button 2(Green): Enable/Disable Roll transmission
    - Button 3(Blue): Enable/Disable Yaw transmission
  - Mode 3:
    - Button 1(Red): Enable/Disable Acceleration X transmission
    - Button 2(Green): Enable/Disable Acceleration Y transmission
    - Button 3(Blue): Enable/Disable Acceleration Z transmission
  - Each button's LED blinks to the corresponding active mode

*~ Under continuous development ~*

## HARDWARE:

For this project I decided to solder the hardware together to have permanent, stable connections. I started with a perf board and designed the possible layouts in order to get the most compact piece of hardware.

**Attempt #1**:

<img src="https://github.com/Amphicheiras/Pixie-Gauntlet/blob/main/media/PX_ATTEMPT_1.jpg" alt="PX_ATTEMPT_1" width="400">

**Attempt #2**:

<img src="https://github.com/Amphicheiras/Pixie-Gauntlet/blob/main/media/PX_ATTEMPT_2.jpg" alt="PX_ATTEMPT_2" width="400">

**Final Layout**:

<img src="https://github.com/Amphicheiras/Pixie-Gauntlet/blob/main/media/PX_BOARD.jpg" alt="PX_BOARD" width="400">

The circuit without the ESP32. You can now see the BNO055 (IMU) (black) and the TP4056 (battery (dis)charger) (blue) ICs.

<img src="https://github.com/Amphicheiras/Pixie-Gauntlet/blob/main/media/PX_WITHOUT_ESP32.jpg" alt="PX_WITHOUT_ESP32" width="400">

Then the casing was designed.

<img src="https://github.com/Amphicheiras/Pixie-Gauntlet/blob/main/media/PX_CASEING.jpg" alt="PX_CASEING" width="400">

The corresponding PCB trace-out would look something like this:

<img src="https://github.com/Amphicheiras/Pixie-Gauntlet/blob/main/media/PX_BOARD_PCB.png?raw=true" alt="PX_BOARD_PCB" width="400">

The above PCB design is not the optimal, but for the hardware in my stock, it did the trick. The next hardware version will be an optimized printed PCB circuit for the UF0 project.

The ESP32 pinout for this project can be seen below:

<img src="https://github.com/Amphicheiras/Pixie-Gauntlet/blob/main/media/PX_ESP32_PINOUT.png?raw=true" alt="PX_ESP32_PINOUT" width="700">

For the battery gauge I make a simple voltage divider using 2x10kΩ resistors. This is necessary because the output voltage of the battery (3.7V - 4,2V) exceeds the maximum Vin (3.3V) of the ESP32 analog input pins. 

## Power Schematic:

<img src="https://github.com/Amphicheiras/Pixie-Gauntlet/blob/main/media/PX_schem_POWER.png" alt="PX_SCHEM_POWER" width="400">

Notice that the V+ of the battery is connected to the switch (S1), and both the gauge and the TP4056's B+ are connected to the other end of the switch, to completely kill the amperage from the system.

*Thought - The battery gauge interferes with the IMU, maybe pull-up resistors for the I2C lines will fix this*

## Now, the soldering..

**Phase #1**: Solder the hardware component to the perf board.

<img src="https://github.com/Amphicheiras/Pixie-Gauntlet/blob/main/media/PX_EARLY_BOARD_1.jpg" alt="PX_EARLY_BOARD_1" width="400">

**Phase #2**: Solder the connections between the hardware components.

<img src="https://github.com/Amphicheiras/Pixie-Gauntlet/blob/main/media/PX_EARLY_BOARD_2.jpg" alt="PX_EARLY_BOARD_2" width="400">

**Phase #3**: Solder the jumper wires for the rest of the connections.

<img src="https://github.com/Amphicheiras/Pixie-Gauntlet/blob/main/media/PX_EARLY_BOARD_3.jpg" alt="PX_EARLY_BOARD_3" width="400">

The final circuit:

<img src="https://github.com/Amphicheiras/Pixie-Gauntlet/blob/main/media/PX_Board_TOP.jpg?raw=true" alt="PX_BOARD_TOP" width="400">
<img src="https://github.com/Amphicheiras/Pixie-Gauntlet/blob/main/media/PX_Board_BOTTOM.jpg?raw=true" alt="PX_BOARD_BOTTOM" width="400">
<img src="https://github.com/Amphicheiras/Pixie-Gauntlet/blob/main/media/PX_Board_FRONT.jpg?raw=true" alt="PX_BOARD_FRONT" width="400">
<img src="https://github.com/Amphicheiras/Pixie-Gauntlet/blob/main/media/PX_Board_BACK.jpg?raw=true" alt="PX_BOARD_BACK" width="400">

## EMBEDDED SOFTWARE:

The Pixie project is a continuation of the UF0 project, aiming for better code practices and motion sensor algorithms. The code structure is now more explicit, with better hierarchy and OOP, still, under development.

The basic components of this project are the:
- Hardware
- Music
- Network
- OS (Operating System)

<img src="https://github.com/Amphicheiras/Pixie-Gauntlet/blob/main/media/PX_src_structure.png" alt="PX_SRC_STRUCTURE">

The hardware components drive, unsurprisingly, the hardware parts of the project.

<img src="https://github.com/Amphicheiras/Pixie-Gauntlet/blob/main/media/PX_src_hardware_components.png" alt="PX_SRC_HARDWARE">

The music components handle MIDI operations and musical theory (soon to come).

**PX_BUTTON**: Handles button operations and logic.

**PX_GYRO**: Handles Inertial Measurement Unit related tasks.

**PX_LED**: Soon to be the LED driver.

**PX_POWER**: Handles the battery gauge system.

<img src="https://github.com/Amphicheiras/Pixie-Gauntlet/blob/main/media/PX_src_music_components.png" alt="PX_SRC_MUSIC">

**PX_MIDI**: Handles MIDI related operations and MIDI-over-WiFi packaging.

The network components handle the WiFi connections and the  webserver functionality.

<img src="https://github.com/Amphicheiras/Pixie-Gauntlet/blob/main/media/PX_src_network_components.png" alt="PX_SRC_NETWORK">

**PX_WEBPAGES**: Holds the HTML pages called from the webserver.

**PX_WEBSERVER**: Handles webserver callbacks.

**PX_WIFI**: Handles WiFi Access Point and Station operations.

The OS components are in charge of the operation of the whole system and hold third party algorithms.

<img src="https://github.com/Amphicheiras/Pixie-Gauntlet/blob/main/media/PX_src_OS_components.png" alt="PX_SRC_OS">

**PX_BLACKMAGIC**: Does things no one knows how they work.

**PX_FSM**: Handles higher level system operations.

**PX_OBJECT**: Soon to be the top abstraction level of the project software.

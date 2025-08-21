# Bluetooth Music LED Strip

## Summary
This project seeks to solve an issue where microphone dependent reactive LED strips cannot separate intended sound from background noise, leading to unwanted reactions from the LED strip. This problem is solved by preprocessing the music for peaks in volume
and/or frequences (bass, treble, etc) as timestamps which are then used to activate an LED strip at each of those timestamps. The downsides are that the LED strip is not truly reactive and is relatively hard to manage since many components are needed.

![Block Diagram](blockdiagram.jpg?raw=true "Block Diagram")

Above represents a simple block diagram of this project
### PC (or any other device running Python)
- Represents a device that can run bt.py. bt.py sends data to the HT-08 module at certain timestamps of the loaded song. bt.py also handles playing the audio, syncing the times the data is sent to the HT-08 module with the playback of the audio.
### HC-08 Module
- HC-08 receives data through Bluetooth which then can be transferred to the microcontroller using USART since TX and RX lines are connected to this module. 
### STM32 Microcontroller
- The microcontroller handles the incoming data from HC-08 and drives the communication to WS2812b. Regarding the incoming data, the code only cares about when exactly the data comes so that it can activate the LED strip to mimic the feeling of "reactiveness." When incoming data is detected, the microcontroller powers on the WS2812b strip to red and linearly fades it to off.
### WS2812b
- This is the LED strip that flashes in beat with the audio playback.


---

## Demo
https://youtu.be/bkHBm-ZQEqo

---

## Technical Highlights
- **Platform:** STM32 NUCLEOC031C6
- **Language:** Python, C
- **Peripherals:** PWM, GPIO, USART, DMA

---

---
## Setup

### Wiring Diagram
![Wiring Diagram](wiringdiagram.jpg?raw=true "Wiring Diagram")

### Peripherals
- STM32 NUCLEOC031C6 microcontroller
- WS2812b LED strip
- HC-08 Bluetooth Module
  
### Software Requirements
- STM32CubeIDE
- STM32CubeMX
- HAL
- Python 3.12

### Build & Flash
1. Open project in STM32CubeIDE
2. Connect board via ST-Link port
3. Build
4. Flash

### Usage
1. Open bt.py on device able to run Python and has Bluetooth capabilities.
2. Load desired songs in .wav format into songs folder.
3. Ensure that Bluetooth is turned on.
4. Run bt.py, wait for connection to HC-08, and input song to play.
---
## Vision
Custom PCB that connects to device (phone, laptop, etc) through custom software, which will make strip configurable in color, brightness, etc. This software would connect to the strip and to an audio source for extra volume, if needed.
## Issues/Improvements
1. Implement input to adjust delay in real time to sync up music with LED
2. More compact design
3. Playlist to store 
4. Area to store preexisting timestamps

# Bluetooth Music LED Strip

## Summary
This project seeks to solve an issue where microphone dependent reactive LED strips cannot separate intended sound from background noise, leading to unwanted reactions from the LED strip. This problem is solved by preprocessing the music for peaks in volume
and/or frequences (bass, treble, etc) as timestamps which are then used to activate an LED strip at each of those timestamps. The downsides are that the LED strip is not truly reactive and is relatively hard to manage since many components are needed.

---

## Demo
something

---

## Technical Highlights
- **Platform:** STM32 NUCLEOC031C6
- **Language:** Python, C
- **Peripherals:** PWM, GPIO, USART, DMA

---

## Skills Demonstrated
- 

---

## Quick Start
### Requirements
- Toolchain: (e.g., `arm-none-eabi-gcc`, PlatformIO, Arduino IDE)
- Libraries: [List exact library versions]
- Hardware: [Board model, sensors, actuators]

### Build & Flash
```bash
python testLED.py

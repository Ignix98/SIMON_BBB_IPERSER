[🇪🇸 Español](./README.es.md) · **🇬🇧 English**

# Implementation of the Simon Game in an Embedded Systems Environment

This repository contains the material produced during the design of the Simon game on a BeagleBone Black (BBB) microprocessor, developed for the course **Physical Interfaces and Embedded Systems** of the **Master’s Degree in Automation and Industrial Computer Science (MUAII)** during the year 2025.

## Project resources
| Section | Description |
|----------------------------|------------------------------------|
| [Code](Codigo/Simon_game_def/src) | Project code |
| [Project images](doc/) | Final Project presentation |

## Overview
This project implements the classic Simon game using a BeagleBone Black. Six arcade buttons with built-in LEDs are used for the colors and for the START/PAUSE functions, a buzzer is used for tones, and two dials adjust speed and response time. The logic is organized into nine states (startup, initial sequence, playback of the game sequence, button reading, pauses, phase change, and victory), each running in its own thread, while GPIO interrupts handle the start and pause buttons. In this way, the game progresses and validates the color sequence concurrently and can be configured in real time.

## Bill of materials
| Component | Specifications | Quantity |
|-----------------------------------|------------------|----------|
| BeagleBone Black (BBB) | — | 1 |
| BBB power supply | 5 V @ 2 A | 1 |
| External power supply | 5 V @ 1 A | 1 |
| Breadboard | — | 2 |
| Arcade button (with internal LED) | — | 6 |
| Potentiometer (dial) | — | 2 |
| Servo motor | — | 2 |
| Buzzer | — | 1 |
| NPN transistor | — | 1 |
| MOSFET | — | 6 |
| Resistor | 5 kΩ | 7 |
| Resistor | 220 Ω | 7 |
| Resistor | 1 kΩ | 6 |
| Resistor | 10 kΩ | 2 |
| Capacitor | 100 nF | 6 |

## Final result
Images of the SIMON game.
<table align="center">
  <tr>
    <td><img src="doc/simon_2.jpeg" width="600"></td>
    <td><img src="doc/simon_3.jpeg" width="700"></td>
  </tr>
</table>

Circuit presentation of the SIMON game.
<table align="center">
  <tr>
    <td><img src="doc/simon_1.jpeg" width="600"></td>
  </tr>
</table>

### Demo video
https://youtube.com/shorts/EQTmY3pivEk?feature=share



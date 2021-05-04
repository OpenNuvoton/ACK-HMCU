# ACK HMCU Port for M451 Series

This directory contains an ACK Host MCU Implementation Core MCU-specific implementation, and ACK Host MCU
sample applications projects targeting Nuvoton M451 series MCUs, for use with
the Keil uVision development environment.

## Directory structure

| Folder Name | Description |
|-|-|
| m451_cube\                |  Nuvoton M451 series BSP |
| ack_m451*.c, ack_m451*.h |  Shared ACK-specific C source code |
| nutiny-m451s\                 |  MCU-specific directories |
| nutiny-m451s\ack.uvmpw        |  Multi-project workspace file for Keil uVision 5, contains all ACK Host MCU sample applications for the MCU |
| nutiny-m451s\applications\\*\      | Projects for ACK Host MCU sample applications |
| nutiny-m451s\applications\common\  | Some driver and resource for board. |
| nutiny-m451s\customloader\    | Custom loader application supporting over-the-air updates |

## Programming Flow

The main() have been added one call to setup(), and calls to loop() on each pass through the main application loop. Each ACK Host MCU sample application therefore includes setup() and loop() functions. The ACK HMCU Implementation Core platform implementation is in ack_m4521_platform.c.

## Wiring

Some pins of ACK connectivity module shield must be connected to NuTiny-M451V board.

### Power-related Pins:

| ACK Connectivity Module shield | NuTiny-M451V |
|-|-|
| ioref | +3.3v |
| +5V | +5V |
| GND | GND |

### Communication-related Pins:

| ACK Connectivity Module shield | NuTiny-M451V |
|-|-|
| RX | P37 |
| TX | P38 |
| INT(Interrupt) | P39 |
| RES(Reset) | P40 |

### Other I/O Pins:

|  NuTiny-M451V I/O Pin | Function Description |
|-|-|
| On-board HeartBeat LED (GPIO output, IO1) | Alexa-connected indicator |
| P28(Defined PWM I/O pin) | SmartLight LED |
| P23(GPIO input) | Restore factory setting |

## Resources

[NUTINY-M451V Board Schematics](https://www.nuvoton.com/resource-download.jsp?tp_GUID=HL0320181128163025)

[Latest Nu-Link Keil driver downloading](https://www.nuvoton.com/opencms/resource-download.jsp?tp_GUID=SW0520101208200142)

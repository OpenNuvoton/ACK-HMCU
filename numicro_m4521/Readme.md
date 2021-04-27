# ACK HMCU Port for M4521 Series

This directory contains an ACK Host MCU Implementation Core MCU-specific implementation, and ACK Host MCU
sample applications projects targeting Nuvoton M4521 series MCUs, for use with
the Keil uVision development environment.

## Directory structure

| Folder Name | Description |
|-|-|
| m4521_cube\                |  Nuvoton M031 series BSP |
| ack_m4521*.c, ack_m4521*.h |  Shared ACK-specific C source code |
| nutiny-m4521s\                 |  MCU-specific directories |
| nutiny-m4521s\ack.uvmpw        |  Multi-project workspace file for Keil uVision 5, contains all ACK Host MCU sample applications for the MCU |
| nutiny-m4521s\applications\\*\      | Projects for ACK Host MCU sample applications |
| nutiny-m4521s\applications\common\  | Some driver and resource for board. |
| nutiny-m4521s\customloader\    | Custom loader application supporting over-the-air updates |

## Programming Flow

The main() have been added one call to setup(), and calls to loop() on each pass through the main application loop. Each ACK Host MCU sample application therefore includes setup() and loop() functions. The ACK HMCU Implementation Core platform implementation is in ack_m4521_platform.c.

## Wiring

Some pins of ACK connectivity module shield must be connected to NuTiny-M4521S board.

### Power-related Pins:

| ACK Connectivity Module shield | NuTiny-M4521S |
|-|-|
| ioref | +3.3v |
| +5V | +5V |
| GND | GND |

### Communication-related Pins:

| ACK Connectivity Module shield | NuTiny-M4521S |
|-|-|
| RX | P37 |
| TX | P38 |
| INT(Interrupt) | P39 |
| RES(Reset) | P40 |

### Other I/O Pins:

| I/O Pin | NuTiny-M4521S |
|-|-|
| HeartBeat LED(Alexa-connected indicator) | On-board LED(GPIO output, IO1) |
| SmartLight LED | P28(Defined PWM I/O pin) |
| Restore Button(Factory restore button) | P23(GPIO input) |

## Resources

[M4521 Technical reference manual](https://www.nuvoton.com/export/resource-files/TRM_M4521_Series_EN_Rev1.00.pdf)

[NUTINY-M4521S Board Schematics](https://www.nuvoton.com/resource-download.jsp?tp_GUID=HL0320181128163025)

[Latest Nu-Link Keil driver downloading](https://www.nuvoton.com/opencms/resource-download.jsp?tp_GUID=SW0520101208200142)

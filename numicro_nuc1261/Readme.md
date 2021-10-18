# ACK HMCU Port for NUC1261 Series

This directory contains an ACK Host MCU Implementation Core MCU-specific implementation, and ACK Host MCU
sample applications projects targeting Nuvoton NUC1261 series MCUs, for use with
the Keil uVision development environment.

## Directory structure

| Folder Name | Description |
|-|-|
| libraries\                  |  Nuvoton NUC1261 series BSP |
| ack_numicro*.c, ack_numicro*.h |  Shared ACK-specific C source code |
| nutiny-nuc1261s\                 |  MCU-specific directories |
| nutiny-nuc1261s\ack.uvmpw        |  Multi-project workspace file for Keil uVision 5, contains all ACK Host MCU sample applications for the MCU |
| nutiny-nuc1261s\applications\\*\      | Projects for ACK Host MCU sample applications |
| nutiny-nuc1261s\applications\common\  | Some driver and resource for board. |
| nutiny-nuc1261s\customloader\    | Custom loader application supporting over-the-air updates |

## Programming Flow

The main() have been added one call to setup(), and calls to loop() on each pass through the main application loop. Each ACK Host MCU sample application therefore includes setup() and loop() functions. The ACK HMCU Implementation Core platform implementation is in ack_nuc1261_platform.c.

## Wiring

Some pins of ACK connectivity module shield must be connected to NuTiny-NUC1261S board.

### Power-related Pins:

| ACK Connectivity Module shield | NuTiny-NUC1261S |
|-|-|
| ioref | +3.3v |
| +5V | +5V |
| GND | GND |

### Communication-related Pins:

| ACK Connectivity Module shield | NuTiny-NUC1261S |
|-|-|
| RX | P37 |
| TX | P38 |
| INT(Interrupt) | P39 |
| RES(Reset) | P40 |

### Other I/O Pins:

|  NuTiny-NUC1261S I/O Pin | Function Description |
|-|-|
| On-board HeartBeat LED (GPIO output, LED) | Alexa-connected indicator |
| P28(Defined PWM I/O pin) | SmartLight LED |
| P23(GPIO input) | Restore factory setting |

## Resources

[NUC1261 Technical reference manual]()

[NUTINY-NUC1261S Board Schematics]()

[Latest Nu-Link Keil driver downloading](https://www.nuvoton.com/opencms/resource-download.jsp?tp_GUID=SW0520101208200142)

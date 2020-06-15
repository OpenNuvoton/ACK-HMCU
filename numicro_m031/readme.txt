This directory contains an ACK Host MCU Implementation Core MCU-specific implementation, and ACK Host MCU
sample applications projects targeting Nuvoton M031SE3AE and M032SE3AE MCUs, for use with
the Keil uVision development environment. For M031SE3AE and M032SE3AE, the specific targets are the NuMaker-M031SE and NuMaker-M032SE
boards.

The directory structure is as follows.

    user\platform\numicro_m031\                 this directory
	m031_cube				Nuvoton M031 series BSP
        ack_m031*.c, ack_m031*.h                Shared ACK-specific C source code
        numaker-m03*se*\                        MCU-specific directories 
            ack.uvmpw                           multi-project workspace file for Keil uVision 5;
                                                contains all ACK Host MCU sample applications for the MCU
            applications\*\                     projects for ACK Host MCU sample applications
            applications\common\  	        some driver and resource for board.
            customloader\                       Custom loader application supporting over-the-air updates

To use an M031 port, install the Keil tools (see link at the bottom of this file) and open
one of the ack.uvmpw multi-project workspace files.

The main() have been added one call to setup(), and calls to loop() on each pass through the main application loop. 
Each ACK Host MCU sample application therefore includes setup() and loop() functions.

The ACK HMCU Implementation Core platform implementation is in ack_m031_platform.c.

The ACK connectivity module shield must be mounted on NuMaker-M032SE board. 
The three power-related pins (ioref, 5v, gnd) are the same on both boards. On the bottom left Arduino
header on both boards:

           NuMaker-M032SE board             ACK Connectivity Module shield
    ioref  [ ][x][ ][ ][ ][ ][ ][ ] <-----> [ ][x][ ][ ][ ][ ][ ][ ]
      +5v  [ ][ ][ ][ ][x][ ][ ][ ] <-----> [ ][ ][ ][ ][x][ ][ ][ ]
      gnd  [ ][ ][ ][ ][ ][x][ ][ ] <-----> [ ][ ][ ][ ][ ][x][ ][ ]

The UART and host-interrupt pin locations differ between the two boards. The headers shown below refer to
the upper 2 Arduino headers on the NuMaker-M032SE board, and the upper right Arduino header on the
ACK connectivity module shield.

For ACK connectivity module
              NuMaker-M032SE board	                                       ACK Connectivity Module shield
       reset  [ ][ ][ ][ ][ ][ ][ ][ ][ ][ ]  [ ][ ][ ][ ][x][ ][ ][ ] <-----> [ ][ ][ ][ ][x][ ][ ][ ] 	reset  
    host-int  [ ][ ][ ][ ][ ][ ][ ][ ][ ][ ]  [ ][ ][ ][ ][ ][x][ ][ ] <-----> [ ][ ][ ][ ][ ][x][ ][ ] 	host-int  
     uart-tx  [ ][ ][ ][ ][ ][ ][ ][ ][ ][ ]  [ ][ ][ ][ ][ ][ ][x][ ] <-----> [ ][ ][ ][ ][ ][ ][x][ ]		uart-rx
     uart-rx  [ ][ ][ ][ ][ ][ ][ ][ ][ ][ ]  [ ][ ][ ][ ][ ][ ][ ][x] <-----> [ ][ ][ ][ ][ ][ ][ ][x]		uart-tx

For LEDs connectivity 
           	NuMaker-M032SE board						LEDs					   
   	[ ][ ][ ][ ][ ][ ][ ][ ][x][ ]  [ ][ ][ ][ ][ ][ ][ ][ ]	ACK_SAMPLE_APPLICATIONS_LED_PIN  
   	[ ][ ][ ][ ][ ][ ][ ][ ][ ][x]  [ ][ ][ ][ ][ ][ ][ ][ ]	ACK_SAMPLE_APPLICATIONS_LED_PIN_1
    	[ ][ ][ ][ ][ ][ ][ ][ ][ ][ ]  [x][ ][ ][ ][ ][ ][ ][ ]	ACK_SAMPLE_APPLICATIONS_LED_PIN_2  
    	[ ][ ][ ][ ][ ][ ][ ][ ][ ][ ]  [ ][x][ ][ ][ ][ ][ ][ ]	ACK_SAMPLE_APPLICATIONS_LED_PIN_3  

    		LEDR1 (on-board)					HeartBeat(Alexa-connected indicator)


For Button connectivity:
           	NuMaker-M032SE board						Button		   
    		A0 of UNO interface					For doing restore factory setting


Link to Keil tools for Nuvoton Numicro:
http://www2.keil.com/nuvoton/M0-M23

Technical reference manual:
http://www.nuvoton.com/resource-files/TRM_M031_Series_EN_Rev1.03.pdf

Board user manual:
http://www.nuvoton.com/resource-files/UM_NuMaker-M032SE_EN_Rev1.00.pdf

Latest Nu-Link Keil driver download:
https://www.nuvoton.com/opencms/resource-download.jsp?tp_GUID=SW0520101208200142

# Instructions for working with the OTA/Custom Loader support:

## In ack_numicro_ota_config.h, adjust configuration for your specific NUC1261 series. NUC1261SG4AE (256K flash, 20K RAM).

## Configure ROM and RAM program regions in the CustomLoader project.

In Keil, this is done in the [Options for CustomLoader - Target "CustomLoader"] dialog, [Target] tab. Near the bottom of the dialog there are IROM1 and IRAM1 memory areas defined.

For NUC1261SG4AE assuming 6K Loader Partition:
```
IROM1: 0x0/0x20000
IRAM1: 0x20000000/0x5000
```
The above are calculated as follows.
```
IROM1 starts at the base address of flash memory (always 0x0 on NuMicro MCUs).
IROM1 size is the full amount of ROM on the MCU.
IRAM1 starts at the base address of RAM (always 0x20000000 on NuMicro MCUs).
IRAM1 size is the full amount of RAM on the MCU.
```

## Configure ROM and RAM program regions in the HMCU app.

In Keil, this is done in the [Options for xxx - Target "xxx"] dialog, [Linker] tag.
Disable [Use memory Layer from Target Dialog], and specify scatter file path to [../../common/Misc/ack_hmcu_example.sct] Near the bottom of the dialog there are IROM1 and IRAM1 memory areas defined.

The above are calculated as follows (see ack_numicro_ota_config.h for the values referenced in these descriptions).
```
IROM1 starts at the end of the Status Partition.
IROM1 size is the total amount of flash minus the sizes of the Loader and Status Partitions, divided by 2.
IRAM1 starts at 0x20000000.
IRAM1 size is the amount of RAM in the MCU.
```

## Amazon Important Python scripts:
| Python | Function description |
|-|-|
| hexmerge.py | Merges 2 .hex files into one. Useful to create a single .hex file containing both the Custom Loader application and the HMCU application, to program in the factory. (DO NOT USE for creating OTA images. OTA images must contain ONLY the HMCU application!)
| hex2ota.py | Creates a file suitable for uploading as an HMCU OTA image, from a .hex file that was created by building the HMCU application. |
| hexdump.py | Dump .hex file |
| otadump.py | Dump .ota file |

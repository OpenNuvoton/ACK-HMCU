Instructions for working with the OTA/Custom Loader support:

(1) In ack_m031_ota_config.h, adjust configuration for your specific M031 series. M032SE3AE (128K flash, 16K RAM).

(2) Configure ROM and RAM program regions in the CustomLoader project.

    In Keil, this is done in the [Options for CustomLoader - Target "CustomLoader"] dialog, [Target] tab.
    Near the bottom of the dialog there are IROM1 and IRAM1 memory areas defined.

    For M032SE3AE assuming 11.5K Loader Partition:
        IROM1: 0x0/0x20000
        IRAM1: 0x20000000/0x4000

    The above are calculated as follows.
        IROM1 starts at the base address of flash memory (always 0x0 on NuMicro MCUs).
        IROM1 size is the full amount of ROM on the MCU.
        IRAM1 starts at the base address of RAM (always 0x20000000 on NuMicro MCUs).
        IRAM1 size is the full amount of RAM on the MCU.

(3) Configure ROM and RAM program regions in the HMCU app.

    In Keil, this is done in the [Options for xxx - Target "xxx"] dialog, [Linker] tag.
    Disable [Use memory Layer from Target Dialog], and specify scatter file path to '../../common/Misc/ack_hmcu_example.sct'
    Near the bottom of the dialog there are IROM1 and IRAM1 memory areas defined.

    The above are calculated as follows (see ota_config.h for the values referenced in these descriptions).
        IROM1 starts at the end of the Status Partition.
        IROM1 size is the total amount of flash minus the sizes of the Loader and Status Partitions,
            divided by 2.

        IRAM1 starts at 0x20000000.
        IRAM1 size is the amount of RAM in the MCU.

(4) The output of building a customloader project and of building the HMCU application must both be
    flashed in the factory. There's a hexmerge.py script to enable merging the two hex files into one,
    to make this more convenient.

(5) To get an OTA image file to be uploaded to the ACK portal, run hex2ota.py on the .hex file that
    results from building the HMCU application.

(6) Important Python scripts:
    * hexmerge.py: merges 2 .hex files into one. Useful to create a single .hex file containing both
      the Custom Loader application and the HMCU application, to program in the factory. (DO NOT USE
      for creating OTA images. OTA images must contain ONLY the HMCU application!)
    * hex2ota.py: creates a file suitable for uploading as an HMCU OTA image, from a .hex file that was
      created by building the HMCU application.
    * hexdump.py, otadump.py: dump .hex and .ota files, respectively.
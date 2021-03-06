from enum import IntEnum
from bibliopixel.drivers.ledtype import LEDTYPE


class CMDTYPE(IntEnum):
    SETUP_DATA = 1  # config data (LEDTYPE, SPI speed, num LEDs)
    PIXEL_DATA = 2  # raw pixel data will be sent as [R1,G1,B1,R2,G2,B2,...]
    BRIGHTNESS = 3  # data will be single 0-255 brightness value, length must be 0x00,0x01
    GETID = 4
    SETID = 5
    GETVER = 6
    SYNC = 7


SPIChipsets = [
    LEDTYPE.LPD8806,
    LEDTYPE.WS2801,
    LEDTYPE.SM16716,
    LEDTYPE.APA102,
    LEDTYPE.P9813
]

# Chipsets here require extra pixels padded at the end
# Key must be an LEDTYPE
# value a lambda function to calc the value based on numLEDs
BufferChipsets = {
    LEDTYPE.APA102: lambda num: (int(num / 64.0) + 1)
}

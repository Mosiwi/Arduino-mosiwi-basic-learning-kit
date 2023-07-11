import time
from machine import SPI, Pin

class bc7278:
    def __init__(self):
        self.spi = SPI(0,
                       baudrate=60_000,
                       polarity=1,
                       phase=1,
                       bits=8,
                       firstbit=SPI.MSB,
                       sck=Pin(18),
                       mosi=Pin(19),
                       miso=Pin(16))

    def key_value(self):
        buf = bytearray([0, 0])
        self.spi.readinto(buf, 0xff) 
        dat = buf[0]*255 + buf[1]
        return ((~dat) & 0xffff) >> 11


spi = bc7278()
while True:
    print(spi.key_value())
    time.sleep_ms(1000)
    
    
    
    
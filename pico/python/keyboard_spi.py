import time
from machine import Timer, SPI, Pin

# create input pin on GPIO20
keyFlag = Pin(20, Pin.IN)     
keyValue = 0

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


# Create an object of a class.
spi = bc7278()

# Timer interrupt function.
def getKeyValue(t):
    if keyFlag.value() == 0:
        keyValue = spi.key_value()

# Initialize the timer interrupt function.  
Timer(freq=10, mode=Timer.PERIODIC, callback=getKeyValue)

while True:
    if keyValue != 0:
        print(keyValue)
        keyValue = 0
    time.sleep_ms(1000)
    
    
import time                           # Importing a time class
from machine import Timer, SPI, Pin   # Import the Pin, Timer, and SPI class from the machine module.

# create input pin on GPIO20
keyOutPin = Pin(20, Pin.IN)
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
        buf = bytearray([0, 0])             # Create a two-member list buffer
        self.spi.readinto(buf, 0xff) 
        dat = buf[0]*255 + buf[1]
        return ((~dat) & 0xffff) >> 11


# Create an object of a class.
spi = bc7278()

# Timer interrupt function.
def getKeyValue(t):
    global keyValue
    if keyOutPin.value() == 0:
        keyValue = spi.key_value()     # Reading key values

# Initialize the timer interrupt function.  
Timer(freq=10, mode=Timer.PERIODIC, callback=getKeyValue)

while True:
    if keyValue != 0:
        print(keyValue)
        time.sleep_ms(250)
        keyValue = 0
    
    
    
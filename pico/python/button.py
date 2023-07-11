import time
from machine import Pin

p21 = Pin(21, Pin.IN)     # create input pin on GPIO21
p25 = Pin(25, Pin.OUT)    # create output pin on GPIO25

while(1):
    button_value = p21.value()
    if button_value == 1:
        p25.off()                # set pin to "off" (low) level
    else:
        p25.on()                 # set pin to "on" (high) level
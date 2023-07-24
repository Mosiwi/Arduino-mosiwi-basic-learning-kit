import time
from machine import Pin

Button = Pin(21, Pin.IN)     # create input pin on GPIO21
G_LED = Pin(9, Pin.OUT)      # create output pin on GPIO9

while(1):
    button_value = Button.value()
    if button_value == 1:
        G_LED.off()                # set pin to "off" (low) level
    else:
        G_LED.on()                 # set pin to "on" (high) level
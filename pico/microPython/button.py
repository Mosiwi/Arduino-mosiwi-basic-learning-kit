import time                  # Importing a time class
from machine import Pin      # Import the Pin class from the machine module.

Button = Pin(21, Pin.IN)     # create input pin on GPIO21
G_LED = Pin(9, Pin.OUT)      # create output pin on GPIO9

while(1):                    # An infinite loop statement.
    button_value = Button.value()  # Read the key value
    if button_value == 1:          # Check whether the value of the key is 1, if so, turn off the LED.
        G_LED.off()                # Set pin to "off" (low) level
    else:                          # If the value of the key is not 1, the LED is turned on.
        G_LED.on()                 # Set pin to "on" (high) level
from machine import Timer   # Import the Timer class from the machine module.
from machine import Pin     # Import the Pin class from the machine module.

ledstate = 0;               # Define a global variable
R_LED = Pin(8, Pin.OUT)     # create output pin on GPIO8

def mycallback(t):          # Timed interrupt function
    global ledstate         # Use the "global" keyword to declare that the variable is global, otherwise it is local.
    ledstate = 1 - ledstate # The global variable ledstate is either 0 or 1.
    R_LED.value(ledstate)   # Turn on or off the LED.

# periodic at 1Hz
tim = Timer(freq=1, mode=Timer.PERIODIC, callback=mycallback)

# tim.deinit()              # Stops the timer, and disables the timer peripheral

while True:                 # Always empty loop
    pass
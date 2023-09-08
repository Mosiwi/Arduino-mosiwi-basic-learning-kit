from machine import Timer
from machine import Pin

ledstate = 0;              # Define a global variable
R_LED = Pin(8, Pin.OUT)    # create output pin on GPIO8

def mycallback(t):
    global ledstate        # Use the "global" keyword to declare that the variable is global, otherwise it is local.
    ledstate = 1 - ledstate
    R_LED.value(ledstate)

# periodic at 1kHz
tim = Timer(freq=1, mode=Timer.PERIODIC, callback=mycallback)

# tim.deinit()             # Stops the timer, and disables the timer peripheral

while True:
    pass
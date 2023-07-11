from machine import Timer
from machine import Pin

ledstate = 0;
p8 = Pin(8, Pin.OUT)    # create output pin on GPIO25

def mycallback(t):
    global ledstate
    ledstate = 1 - ledstate
    p8.value(ledstate)

# periodic at 1kHz
tim = Timer(freq=1, mode=Timer.PERIODIC, callback=mycallback)

# tim.deinit()             # Stops the timer, and disables the timer peripheral
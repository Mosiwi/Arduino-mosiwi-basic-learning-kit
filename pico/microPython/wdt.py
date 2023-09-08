from machine import Pin, WDT
import time

p25 = Pin(25, Pin.OUT)    # create output pin on GPIO25
p25.on()                  # set pin to "on" (high) level
time.sleep(1)             # sleep for 1 second
p25.off()                 # set pin to "off" (low) level

# enable the WDT with a timeout of 5s (1s is the minimum)
wdt = WDT(timeout=30000)
wdt.feed()

while True:
    pass
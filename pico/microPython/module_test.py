import time                             # Importing the time class
from mosiwi_lib.test import hello, prt  #  Import prt class and hello function from the machine module.

Class = prt()
while True:                      # Always empty loop
    hello()
    Class.hello()
    time.sleep(1)                # sleep for 1 second
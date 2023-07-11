import time
from machine import ADC, Pin

pm = ADC(Pin(28))                    # create ADC object on ADC pin

while(1):
    adc = pm.read_u16()              # read value, 0-65535 across voltage range 0.0v - 3.3v
    time.sleep_ms(500)               # sleep for 500 milliseconds
    print(adc)       
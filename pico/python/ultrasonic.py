import time
from machine import Pin, PWM

echo = Pin(9, Pin.IN)       # create input pin on GPIO9, S1 pin of the expansion board
trig = Pin(8, Pin.OUT)      # create output pin on GPIO8, S2 pin of the expansion board
trig.value(0)

def Measuring_distance():
    c = 0
    # Send a 10us trigger signal to the ultrasonic module.
    trig.value(1)
    time.sleep_us(20)
    trig.value(0)
    
    # Capture the pulse signal of the echo pin of the ultrasonic module.
    while(echo.value() == 0 and c <= 1000):
        time.sleep_us(20)
        c = c + 1
    if c >= 1000:
        return 0.0
    c = 0
    while(echo.value() == 0 and c <= 800 ):
        time.sleep_us(5)
        c = c + 1
    if c >= 800:
        return 0.0
    else:
        return 1.7*c  # Unit is centimeter
    
    
while True:
    distance = Measuring_distance()
    print("Ultrasonic distance measured: %0.1f CM" %distance)
    time.sleep_ms(1000)
    
    
    
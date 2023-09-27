import time                  # Importing a time class
from machine import Pin, PWM # Import the Pin and PWM class from the machine module.

echo = Pin(9, Pin.IN)        # create input pin on GPIO9, S1 pin of the expansion board
trig = Pin(8, Pin.OUT)       # create output pin on GPIO8, S2 pin of the expansion board
trig.value(0)

soundVelocity=34000/1000000  # 0.034cm/1us

def Measuring_distance():
    a = 0
    b = 0
    # Send a 10us trigger signal to the ultrasonic module.
    trig.value(1)
    time.sleep_us(10)
    trig.value(0)
    
    # Capture the pulse signal of the echo pin of the ultrasonic module.
    while(echo.value() == 0 and a <= 1600):
        time.sleep_us(5)
        a = a + 1
    t1=time.ticks_us()
    while(echo.value() == 1 and b <= 800 ):
        time.sleep_us(5)
        b = b + 1
    t2=time.ticks_us()
    if a >= 1600 or b >= 800:
        return 0.0
    else:
        t = time.ticks_diff(t2, t1)
        return (soundVelocity*t)/2  # Unit is centimeter
    
    
while True:
    distance = Measuring_distance()
    print("Ultrasonic distance measured: %0.1f CM" %distance)
    time.sleep_ms(1000)
    
    
    
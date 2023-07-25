import time
from machine import Pin, PWM

# pwd_f  = pwm0.freq()           # get current frequency
# pwm_dy = pwm0.duty_u16()       # get current duty cycle, range 0-65535
# pwm8.deinit()                  # turn off PWM on the pin

S2 = PWM(Pin(8))                 # create PWM object from a pin
S2.freq(1000)                    # set frequency

S1 = PWM(Pin(9))              
S1.freq(1000)

while True:
    # CW
    for i in range(0, 42768):
        S1.duty_u16(i)           # set duty cycle, range 0-65535
        S2.duty_u16(0)       # set duty cycle, range 0-65535
        time.sleep_us(50)        # sleep for 20 microseconds
    for i in range(0, 42768):
        time.sleep_us(50)        
        S1.duty_u16(42767-i)
        S2.duty_u16(0)
    # CCW
    for i in range(0, 42768):
        S2.duty_u16(i)           
        S1.duty_u16(0)       
        time.sleep_us(50)        
    for i in range(0, 42768):
        time.sleep_us(50)        
        S2.duty_u16(42767-i)
        S1.duty_u16(0) 

    time.sleep_ms(1000)
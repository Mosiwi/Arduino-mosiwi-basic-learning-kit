import time                      # Importing a time class
from machine import Pin, PWM     # Import the Pin and PWM class from the machine module.

# pwd_f  = pwm0.freq()           # get current frequency
# pwm_dy = pwm0.duty_u16()       # get current duty cycle, range 0-65535
# pwm8.deinit()                  # turn off PWM on the pin

S2 = PWM(Pin(8))                 # create PWM object from pin8
S2.freq(1000)                    # Set frequency of the PWM

S1 = PWM(Pin(9))                 # create PWM object from pin9             
S1.freq(1000)                    # Set frequency of the PWM

while True:                      # An infinite loop statement.
    # CW
    for i in range(0, 42768):    # The loop executes the next 3 statements 42768 times.
        S1.duty_u16(i)           # Set duty cycle of the pwm, 0 to 42767
        S2.duty_u16(0)           # The duty cycle of the PWM is set to 0.    
        time.sleep_us(50)        # Sleep for 50 microseconds
    for i in range(0, 42768):    # The loop executes the next 3 statements 42768 times.
        time.sleep_us(50)        # Sleep for 50 microseconds  
        S1.duty_u16(42767-i)     # Set duty cycle of the pwm, 42767 to 0
        S2.duty_u16(0)           # The duty cycle of the PWM is set to 0.
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
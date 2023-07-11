import time
from machine import Pin, PWM

# pwd_f  = pwm0.freq()          # get current frequency
# pwm_dy = pwm0.duty_u16()      # get current duty cycle, range 0-65535
# pwm8.deinit()                 # turn off PWM on the pin

pwm8 = PWM(Pin(8))              # create PWM object from a pin
pwm8.freq(1000)                 # set frequency

while True:
    for i in range(0, 65536):
        time.sleep_us(50)       # sleep for 100 microseconds
        pwm8.duty_u16(i)        # set duty cycle, range 0-65535
        
    for i in range(0, 65536):
        time.sleep_us(50)       # sleep for 100 microseconds
        pwm8.duty_u16(65535-i)  # set duty cycle, range 0-65535
        
    time.sleep_ms(1000)
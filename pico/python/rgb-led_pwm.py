import time
from machine import Pin, PWM

# pwd_f  = pwm0.freq()           # get current frequency
# pwm_dy = pwm0.duty_u16()       # get current duty cycle, range 0-65535
# pwm8.deinit()                  # turn off PWM on the pin

R_LED = PWM(Pin(8))              # create PWM object from a pin
R_LED.freq(1000)                 # set frequency

G_LED = PWM(Pin(9))              
G_LED.freq(1000)

B_LED = PWM(Pin(7))              
B_LED.freq(1000) 

while True:
    for i in range(0, 65536):
        time.sleep_us(20)        # sleep for 20 microseconds
        R_LED.duty_u16(i)        # set duty cycle, range 0-65535      
    for i in range(0, 65536):
        time.sleep_us(20)        
        R_LED.duty_u16(65535-i)  

    for i in range(0, 65536):
        time.sleep_us(20)       
        G_LED.duty_u16(i)        
    for i in range(0, 65536):
        time.sleep_us(20)       
        G_LED.duty_u16(65535-i)  

    for i in range(0, 65536):
        time.sleep_us(20)       
        B_LED.duty_u16(i)        
    for i in range(0, 65536):
        time.sleep_us(20)       
        B_LED.duty_u16(65535-i)

    time.sleep_ms(1000)
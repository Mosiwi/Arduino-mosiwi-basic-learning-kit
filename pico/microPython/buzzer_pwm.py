import time                       # Importing a time class
from machine import Pin, PWM      # Import the Pin and PWM class from the machine module.

# pwd_f  = pwm0.freq()            # get current frequency
# pwm_dy = pwm0.duty_u16()        # get current duty cycle, range 0-65535
# pwm8.deinit()                   # turn off PWM on the pin

buzzer = PWM(Pin(6))              # create PWM object from a pin
buzzer.freq(1000)                 # set frequency 

while True:                       # An infinite loop statement.
    for i in range(0, 65536):     # The loop executes the next two statements 65,536 times.
        time.sleep_us(50)         # Sleep for 50 microseconds
        buzzer.duty_u16(i)        # Set duty cycle of the PWM, 0 to 65535

    for i in range(0, 65536):     # The loop executes the next two statements 65,536 times.
        time.sleep_us(50)         # Sleep for 50 microseconds
        buzzer.duty_u16(65535-i)  # Set duty cycle of the PWM, 65535 to 0

    time.sleep_ms(1000)           # Sleep for 100 milliseconds
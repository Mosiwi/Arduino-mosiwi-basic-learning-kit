import time                # Importing a time class
from machine import Pin    # Import the Pin class from the machine module.

class tw:                  # Defining a class
    LSBFIRST = 0           # Define a class variable
    MSBFIRST = 1           # Define a class variable
    led_dat  = 0           # Define a class variable
    def __init__(self, st_cp=13, sh_cp=14, ds=15):      # Constructor for a class
        self._sh = Pin(sh_cp, Pin.OUT)                  # Define pin A pin object
        self._ds = Pin(ds, Pin.OUT)                     # Define pin A pin object
        self._st = Pin(st_cp, Pin.OUT)                  # Define pin A pin object
        self._st.value(1)

    def shift_out(self, bit_order, dat):                # Define a member function of a class
        for i in range(0, 8):
            if bit_order == self.LSBFIRST:
                self._ds.value(dat & 0x01)
                dat >>= 1
            else:
                self._ds.value(dat & 0x80)
                dat <<= 1
                
            self._sh.value(1)
            time.sleep_us(10)
            self._sh.value(0)
            time.sleep_us(10)

    def set_(self, led_bit, on_off):                    # Define a member function of a class
        if on_off == 1:
            self.led_dat = self.led_dat | (0x01 << led_bit)
        else:
            self.led_dat = self.led_dat & (~(0x01 << led_bit))

        self._st.value(0)
        self.shift_out(self.MSBFIRST, self.led_dat)
        self._st.value(1)


led_bar = tw()                  # Defines an object of a class.

while True:
    for i in range(0, 8):       # The eight leds are turned on in sequence. 
        led_bar.set_(i, 1)
        time.sleep_ms(1000)
    for i in range(0, 8):       # The eight leds are turned off in sequence.
        led_bar.set_(8-i, 0)
        time.sleep_ms(1000)
    time.sleep_ms(1000)
    














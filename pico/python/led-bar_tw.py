from machine import Pin
import time

class tw:
    LSBFIRST = 0
    MSBFIRST = 1
    led_dat  = 0
    def __init__(self, st_cp=13, sh_cp=14, ds=15):
        self._sh = Pin(sh_cp, Pin.OUT)
        self._ds = Pin(ds, Pin.OUT)
        self._st = Pin(st_cp, Pin.OUT)
        self._st.value(1)

    def shift_out(self, bit_order, dat):
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

    def set_(self, led_bit, on_off):
        if on_off == 1:
            self.led_dat = self.led_dat | (0x01 << led_bit)
        else:
            self.led_dat = self.led_dat & (~(0x01 << led_bit))

        self._st.value(0)
        self.shift_out(self.MSBFIRST, self.led_dat)
        self._st.value(1)


led_bar = tw()

while True:
    for i in range(0, 8):
        led_bar.set_(i, 1)
        time.sleep_ms(1000)
    for i in range(0, 8):
        led_bar.set_(8-i, 0)
        time.sleep_ms(1000)
    time.sleep_ms(1000)
    














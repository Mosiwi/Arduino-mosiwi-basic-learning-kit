import time
from machine import SPI, Pin

class bc7278:
    DecReg = 0x1B
    SegAddReg = 0x1C
    
    def __init__(self):
        self.spi = SPI(0,
                       baudrate=60_000,
                       polarity=1,
                       phase=1,
                       bits=8,
                       firstbit=SPI.MSB,
                       sck=Pin(18),
                       mosi=Pin(19),
                       miso=Pin(16))

    '''
    * display segment
    * Seg   DP    G    F    E    D    C    B    A
    * Bit 
    *  0    7h    6h   5h   4h   3h   2h   1h   0h
    *  1    fh    eh   dh   ch   bh   ah   9h   8h
    *  2    17h   16h  15h  14h  13h  12h  11h  10h
    *  3    1fh   1eh  1dh  1ch  1bh  1ah  19h  18h
    *  
    * OnOff = 1 = on, OnOff = 0 = off
    '''
    def set_display_seg(self, Seg, OnOff):
        Seg = (((~OnOff) & 0xff) << 7) + Seg;
        buf = bytearray([self.SegAddReg, Seg])
        self.spi.write(buf) 

    '''
    * display: 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, A,  b,  C,  d,  E,  F
    * dat    : 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15
    * dg     : 0, 1, 2, 3
    '''
    def diaplay_bit(self, dg, char):
        buf = bytearray([self.DecReg, (3-dg)*16 + char])
        self.spi.write(buf)       

    '''
    * Clear one bit display.
    * dg = 0--3
    '''
    def clear_bit(self, dg):
        dg = 3-dg
        if dg == 0:
            for i in range(0, 7):
                self.set_display_seg(i, 0)
        elif dg == 1:
            for i in range(8, 15):
                self.set_display_seg(i, 0)     
        elif dg == 2:
            for i in range(16, 23):
                self.set_display_seg(i, 0)
        elif dg == 3:
            for i in range(24, 31):
                self.set_display_seg(i, 0)

    '''
    # decimals = 0: Integer number
    # Integer number: 0---9999
    # decimals = 1: Floating number
    # Floating number: 0---999.9
    '''
    def diaplay_num(self, dat_, decimals = 0):      
        if decimals == 0:
            self.set_display_seg(0x17, 0)  # Turn off the decimal point.
            dat = int(dat_%10000)
        elif decimals == 1:
            self.set_display_seg(0x17, 1)  # Turn on the decimal point.
            dat = int((dat_*10)%10000)

        if dat_ > (999.9 if decimals == 1 else 9999):
            for i in range(0, 4):
                self.diaplay_bit(i, 15)
            return
        
        if int(dat/1000) != 0:
            self.diaplay_bit(3, int(dat/1000))
        else:
            self.clear_bit(3)
        if (int(dat/1000) != 0) and (int(dat%1000/100) != 0):
            self.diaplay_bit(2, int(dat%1000/100))
        else:
            self.clear_bit(2)
        if (int(dat/1000) != 0) and (int(dat%1000/100) != 0) and (int(dat%100/10 != 0)):
            self.diaplay_bit(1, int(dat%100/10))
        else:
            self.clear_bit(1)          
        self.diaplay_bit(0, int(dat%10))

        
spi = bc7278()
while True:
    for a in range(0, 9999):
        spi.diaplay_num(a)
        time.sleep_ms(1000)
    spi.diaplay_num(999.9, 1)
    time.sleep_ms(1000)






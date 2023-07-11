'''
 * Project: AHT20 hardware i2c
 * Function: Read temperature and humidity
 * Github: https://github.com/mosiwi
 * Wiki: https://mosiwi-wiki.readthedocs.io/en/latest/index.html
 * Web: http://mosiwi.com
 * Engineer: Jalen
 * date: 2023-4-13
'''

import time
from machine import Pin, I2C, SPI

class crc:
    def __init__(self):
        pass
    
    def crc8_cw(self, buf, _len):
        crc=0x00
        for cnt in range(0, _len):
            crc ^= buf[cnt]
            for i in range(0, 8):
                if (crc & 0x01) == 0x01:
                    crc=(crc>>1)^0x8c
                else:
                    crc=(crc>>1)
        return (crc & 0xff)

    def crc8_ccw(self, buf, _len):
        crc=0xff
        for cnt in range(0, _len):
            crc ^= buf[cnt]
            for i in range(0, 8):
                if crc & 0x80 == 0x80:
                    crc=(crc<<1)^0x31
                else:
                    crc=(crc<<1)
        return (crc & 0xff)
    
    def crc16(self, buf, _len, inverted_crc):
        crc=0x00
        for cnt in range(0, _len):
            crc ^= buf[cnt]
            for i in range(0, 8):
                if (crc & 0x01) == 0x01:
                    crc = (crc >> 1) ^ 0xa001
                else:
                    crc = (crc >> 1)
        crc = ~(crc & 0xffff)
        return (crc & 0xff == inverted_crc[0]) and ((crc >> 8) & 0xff == inverted_crc[1])


class aht20(crc):
    # AHT20 Register address
    reg1_ = 0x1b
    reg2_ = 0x1c
    reg3_ = 0x1e
    ac_   = 0xac
    ac_d1 = 0x33
    ac_d2 = 0x00
    
    def __init__(self, scl_=5, sda_=4, addr=0x38):
        self.address = addr
        self.i2c = I2C(0, scl=Pin(scl_), sda=Pin(sda_), freq=400_000)
        #crc.__init__(self)
    
    def sendac(self):
        buf = bytearray([self.ac_, self.ac_d1, self.ac_d2]) 
        self.i2c.writeto(self.address, buf)
        
    def read_ctdata(self, ct):
        Byte = bytearray([0, 0, 0, 0, 0, 0, 0])
        CTDATA = [0, 0, 0, 0, 0, 0]
        RetuData = 0
        cnt = 0
        
        self.sendac()
        time.sleep_ms(80)
        
        while self.read_status() & 0x80 == 0x80:
            time.sleep_ms(2)
            if cnt >= 100:
                print("Error, busy:" + str(self.read_status()))
                break
            cnt += 1
        
        self.i2c.readfrom_into(self.address, Byte)
        CTDATA = Byte[0:6]
        
        if crc.crc8_ccw(self, CTDATA, 6) == Byte[6]:
            RetuData = (RetuData|Byte[1])<<8
            RetuData = (RetuData|Byte[2])<<8
            RetuData = (RetuData|Byte[3])
            RetuData =RetuData >>4;
            ct[0] = float(RetuData)*100/1024/1024           #humidity 
            RetuData = 0;
            RetuData = (RetuData|Byte[3])<<8
            RetuData = (RetuData|Byte[4])<<8
            RetuData = (RetuData|Byte[5])
            RetuData = RetuData&0xfffff
            ct[1] =float(RetuData)*200/1024/1024-50        #temperature
        else:
            ct[0]=0x00
            ct[1]=0x00
            print("Error, crc8!")
        
    def reset_reg(self, reg):
        Byte = bytearray([reg, 0x00, 0x00])                #Byte.append(1)       
        self.i2c.writeto(self.address, Byte)           
        
        time.sleep_ms(5)
        self.i2c.readfrom_into(self.address, Byte)
        
        time.sleep_ms(10)
        Byte[0] = 0xb0|reg
        self.i2c.writeto(self.address, Byte) 

    def read_status(self):
        sta = self.i2c.readfrom(self.address, 1)
        return sta[0]
    
    def start_init(self):
        if (self.read_status() & 0x18) != 0x18:
            self.reset_reg(0x1b);
            self.reset_reg(0x1c);
            self.reset_reg(0x1e);



class stc:
    def __init__(self, scl=5, sda=4, addr=0x2D):
        self.address = addr
        self.i2c = I2C(0, scl=Pin(5), sda=Pin(4), freq=100_000)
    
    def read_data_from_stc(self, chan):
        buf = bytearray([chan]) 
        self.i2c.writeto(self.address, buf, False)
        buf_ = bytearray([0, 0])
        self.i2c.readfrom_into(self.address, buf_)
        time.sleep_ms(5)
        return buf_[0]*256 + buf_[1]
        
    def get_ir_data(self):
        return self.read_data_from_stc(0x04)

    def get_sr_data(self):
        return self.read_data_from_stc(0x02)
    
    def get_mic_data(self):
        return self.read_data_from_stc(0x00)
        


class bc7278:
    DecReg = 0x1B
    SegAddReg = 0x1C
    
    def __init__(self):
        self.spi = SPI(0, baudrate=60_000, polarity=1, phase=1,
                       bits=8, firstbit=SPI.MSB, sck=Pin(18),
                       mosi=Pin(19), miso=Pin(16))

    def key_value(self):
        buf = bytearray([0, 0])
        self.spi.readinto(buf, 0xff) 
        dat = buf[0]*255 + buf[1]
        return ((~dat) & 0xffff) >> 11

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
        if (int(dat/1000) != 0) or (int(dat%1000/100) != 0):
            self.diaplay_bit(2, int(dat%1000/100))
        else:
            self.clear_bit(2)
        if (int(dat/1000) != 0) or (int(dat%1000/100) != 0) or (int(dat%100/10 != 0)):
            self.diaplay_bit(1, int(dat%100/10))
        else:
            self.clear_bit(1)          
        self.diaplay_bit(0, int(dat%10))



class ow(crc):
    # commands
    WRITE_SCRATCHPAD = 0x0F
    READ_SCRATCHPAD = 0xAA
    COPY_SCRATCHPAD = 0x55
    READ_MEMORY = 0xF0
    
    ONE_WIRE_MAC_SIZE = 8
    ONE_WIRE_FAMILY_CODE = 0x2D
    EEPROM_SIZE = 128
    ROW_SIZE = 8
    
    PF_MASK = 0x07
    WRITE_MASK = 0xAA
    CMD_SIZE = 3
    CRC_SIZE = 2
    READ_RETRY = 2
  
    def __init__(self, pin=11):
        self._ow = onewire.OneWire(Pin(11))   # create a OneWire bus on GPIO11
        #crc.__init__(self)      
        
    def scan(self):
        self.roms = self._ow.scan()           # return a list of devices on the bus
        return self.roms

    def read(self, rom, addr, buf, _len):
        self._ow.reset()                      # reset the bus
        self._ow.select_rom(rom)
    
        com = bytearray([self.READ_MEMORY, addr & 0xff, (addr>>8) & 0xff])
        self._ow.write(com)
        for i in range(0, _len):
            buf[i] = self._ow.readbyte()

    def write(self, rom, addr, buf, _len):
        wbuf = bytearray(self.CMD_SIZE)
        crc16 = bytearray(self.CRC_SIZE)
        
        wbuf[0] = self.WRITE_SCRATCHPAD
        wbuf[1] = addr & 0xff
        wbuf[2] = (addr>>8) & 0xff
        wbuf = wbuf + buf

        self._ow.reset()                      # reset the bus
        self._ow.select_rom(rom)
        self._ow.write(wbuf)       
        for i in range(0, self.CRC_SIZE):
            crc16[i] = self._ow.readbyte()      

        verlify = False
        if crc.crc16(wbuf, len(wbuf), crc16) == False:
            verlify = True

        rbuf = bytearray(12)
        rbuf[0] = self.READ_SCRATCHPAD
        for i in range(0, 3):
            self._ow.reset()                  # reset the bus
            self._ow.select_rom(rom)           
            self._ow.writebyte(rbuf[0])
            for i in range(1, self.CMD_SIZE+1):
                rbuf[i] = self._ow.readbyte()       
        
            if rbuf[3] != self.PF_MASK:
                verlify = True
 
            if verlify:
                for i in range(4, _len+4):
                    rbuf[i] = self._ow.readbyte()
                for i in range(0, self.CRC_SIZE):
                    crc16[i] = self._ow.readbyte()                   
                if crc.crc16(rbuf, 12, crc16): 
                    continue
                if addr != ((rbuf[2]<<8) + rbuf[1]):
                    return False
                if rbuf[3] != self.PF_MASK:
                    return False
                if rbuf[4: ] == buf:
                    return False
                break
        
        rbuf[0] = self.COPY_SCRATCHPAD
        
        self._ow.reset()                  # reset the bus
        self._ow.select_rom(rom)
        self._ow.write(rbuf[0 : self.CMD_SIZE + 1])
        time.sleep_ms(15)                 # The necessary delay
        
        res = self._ow.readbyte()
        if res != self.WRITE_MASK:
            return False
        
        return True
    
    
    
ht = aht20()
ht.start_init()

iic = stc()
spi = bc7278()
_ow = ow()

while True:
    # i2c: aht20
    ht_data = [0,0]
    ht.read_ctdata(ht_data)
    print("Humidity = " + str(ht_data[0]))
    print("Temperature = " + str(ht_data[1]) +"\n")
    time.sleep_ms(1000)
    
    # i2c stc
    mic_data = iic.get_mic_data()
    print("SR value = " + str(mic_data))
    sr_data = iic.get_sr_data()
    print("SR value = " + str(sr_data))
    ir_data = iic.get_ir_data()
    print("IR value = " + str(ir_data) + "\n")
    time.sleep_ms(1000)
    
    # spi: bc7278
    spi.diaplay_num(999.8, 1)
    print("4-digit nixie tube display 999.8")
    print("Button value = " + str(spi.key_value()) + "\n")
    time.sleep_ms(1000)    
    
    # one wire: memory
    # 1. Search the 1-Wire bus for a connected device.
    _roms = _ow.scan()                         
    if len(_roms) == 0:
        print("No one wire device found!")
        while True:
            pass
    print("Device rom: " + str(_roms) + "\n")
    
    # 2. Select the first device and verify the series number with CRC8.
    if _ow.crc8_cw(_roms[0], 7) != _roms[0][7]:   
        print("Rom crc err!")
        while True:
            pass
    
    # 3. Write data to memory.
    buf_wdat = bytearray([1,2,3,4,5,6,7,8])
    n = 0        # n = 0 --- 15
    addr = 8*n
    if _ow.write(_roms[0], addr, buf_wdat, len(buf_wdat)) == False:
        while True:
            pass
    print("Write to address 0: " + str(buf_wdat) + "\n")
    
    # 4. Read data from memory
    rdat = bytearray(8)
    _ow.read(_roms[0], 0, rdat, len(rdat))
    print("Read from address 0: " + str(rdat) + "\n")
    
    time.sleep_ms(1000)
    
    
    
    
    
    

'''
 * Project: AHT20 hardware i2c
 * Function: Read temperature and humidity
 * Github: https://github.com/mosiwi
 * Wiki: https://mosiwi-wiki.readthedocs.io/en/latest/index.html
 * Web: http://mosiwi.com
 * Engineer: Jalen
 * date: 2023-4-13
'''

import time                      # Importing a time class
from machine import Pin, I2C     # Import the Pin and PWM class from the machine module.

# i2c.deinit()      # Turn off the I2C bus

class aht20:                     # Create an aht20 class
    # AHT20 Register address
    reg1_ = 0x1b
    reg2_ = 0x1c
    reg3_ = 0x1e
    ac_   = 0xac
    ac_d1 = 0x33
    ac_d2 = 0x00
    
    def __init__(self, scl_=5, sda_=4, addr=0x38):    # Class constructor
        self.address = addr                           # Create a class variable and assign the 
                                                      # i2c slave address of aht20 to it.
        # Create an aht20 i2c object and set the communication frequency to 100KHz.
        self.i2c = I2C(0, scl=Pin(scl_), sda=Pin(sda_), freq=100_000)
        
    def calc_crc8(self, message, Num):                # CRC8 check function
        crc=0xFF
        for cnt in range(0, Num):
            crc ^= message[cnt]
            for i in range(0, 8):
                if crc & 0x80 == 0x80:
                    crc=(crc<<1)^0x31
                else:
                    crc=(crc<<1)
        return (crc & 0x000000ff)
        
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
        
        if self.calc_crc8(CTDATA, 6) == Byte[6]:
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
        
        
ht = aht20()       # Create an object of class aht20.
ht.start_init()    # Initialize the status of aht20 temperature and humidity sensor.

while True:
    time.sleep_ms(1000)
    ht_data = [0,0]
    ht.read_ctdata(ht_data)
    print("Humidity = " + str(ht_data[0]))
    print("Temperature = " + str(ht_data[1]))
        
        
        
        

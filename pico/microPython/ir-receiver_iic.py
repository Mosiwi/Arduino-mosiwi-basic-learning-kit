import time
from machine import Pin, I2C

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
        
iic = stc()

while True:
    #sr_data = iic.get_sr_data()
    #mic_data = iic.get_mic_data()
    ir_data = iic.get_ir_data()
    if ir_data != 0:
        print("Remote key value: %d" %ir_data)
    time.sleep_ms(100)





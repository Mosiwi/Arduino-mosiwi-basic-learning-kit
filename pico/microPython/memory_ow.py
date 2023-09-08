from machine import Pin
import time, onewire

class ow:
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
        
    def crc8(self, buf, _len):
        crc=0x00
        for cnt in range(0, _len):
            crc ^= buf[cnt]
            for i in range(0, 8):
                if (crc & 0x01) == 0x01:
                    crc=(crc>>1)^0x8c
                else:
                    crc=(crc>>1)
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
        if _ow.crc16(wbuf, len(wbuf), crc16) == False:
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
                if _ow.crc16(rbuf, 12, crc16): 
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
        
        
_ow = ow()

while True:
    # 1. Search the 1-Wire bus for a connected device.
    _roms = _ow.scan()                         
    if len(_roms) == 0:
        print("No one wire device found!")
        while True:
            pass
    print("Device rom: " + str(_roms) + "\n")
    
    # 2. Select the first device and verify the series number with CRC8.
    if _ow.crc8(_roms[0], 7) != _roms[0][7]:   
        print("Rom crc err!")
        while True:
            pass

    '''
    # 3.Read all memory content
    buf_rdat = bytearray(128)
    _ow.read(_roms[0], 0, buf_rdat, len(buf_rdat))
    print("Memory content: ")
    print(str(buf_rdat) + "\n")
    '''
    
    # 4. Write data to memory.
    buf_wdat = bytearray([1,2,3,4,5,6,7,8])
    n = 0        # n = 0 --- 15
    addr = 8*n
    if _ow.write(_roms[0], addr, buf_wdat, len(buf_wdat)) == False:
        while True:
            pass
    print("Write to address 0: " + str(buf_wdat) + "\n")
    
    # 5. Read data from memory
    rdat = bytearray(8)
    _ow.read(_roms[0], 0, rdat, len(rdat))
    print("Read from address 0: " + str(rdat) + "\n")
    
    time.sleep_ms(1000)
    







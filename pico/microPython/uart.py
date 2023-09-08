import time
from machine import UART, Pin

uart0 = UART(0, baudrate=9600, tx=Pin(0), rx=Pin(1))
while(1):
    uart0.write('Mosiwi!\n')       # write bytes
    time.sleep_ms(1000) 
    string = uart0.read()          # read bytes
    if(str(string) != 'None'):     
        print(string)
    time.sleep_ms(1000) 
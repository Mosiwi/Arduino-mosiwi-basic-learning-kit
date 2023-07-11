from machine import UART, Pin

uart0 = UART(0, baudrate=9600, tx=Pin(0), rx=Pin(1))
uart0.write('Uart0 OK!\n')           # write 9 bytes
while(1):
    string = uart0.read()            # read up to 5 bytes
    if(str(string) != 'None'):
        uart0.write(str(string)+'\n')       # write 5 bytes
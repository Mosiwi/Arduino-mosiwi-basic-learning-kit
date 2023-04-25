/*
 * Project: AHT20 hardware i2c
 * Function: Read temperature and humidity
 * Wiki: https://mosiwi-wiki.readthedocs.io
 * Web: http://mosiwi.com
 * Engineer: Jalen
 * date: 2022-3-11
 */

#include <Wire.h>

int addr_ = 0x38;        //device address

int reg1_ = 0x1b;
int reg2_ = 0x1c;
int reg3_ = 0x1e;

int ac_   = 0xac;
int ac_d1 = 0x33;
int ac_d2 = 0x00;


//CRC：CRC8/MAXIM
//polynomial：X8+X5+X4+1
//Poly：0011 0001  0x31
//If I put the higher order in the back, it becomes: 1000 1100 0x8c
byte Calc_CRC8(byte *message, byte Num)
{
  byte i;
  byte byte_;
  byte crc=0xFF;
  for(byte_=0; byte_<Num; byte_++){
    crc^=(message[byte_]);
    for(i=8;i>0;--i){
      if(crc&0x80) crc=(crc<<1)^0x31;
      else crc=(crc<<1);
    }
  }
  return crc;
}


void AHT20_SendAC(void){
  Wire.beginTransmission(addr_);  // transmit to device 
  Wire.write(ac_);                // sends value byte 
  Wire.write(ac_d1);
  Wire.write(ac_d2);
  Wire.endTransmission();         // stop transmitting  
}


//No CRC check, read AHT20 temperature and humidity data directly
void AHT20_Read_CTdata(float *ct){
  byte  Byte[6] = {0,0,0,0,0,0};
  long  RetuData = 0;
  byte  cnt = 0;  

  AHT20_SendAC();                // Send the AC command to AHT20
  delay(80);

  //Until bit[7] is 0, indicating idle state. If it is 1, indicating busy state
  while(((AHT20_Read_Status()&0x80)==0x80)){
    delay(2);
    if(cnt++>=100){
     break;
     }
  }
  Wire.requestFrom(addr_, 6);    // request 1 bytes from slave device
  cnt = 0;
  while(Wire.available()){       // slave may send less than requested  
    Byte[cnt] = Wire.read();     // receive a byte as character
    cnt++;
  }

  // Byte[0]  //Status word: the state is 0x98, indicating busy state, and bit[7] is 1.  The state is 0x1C, or 0x0C, or 0x08 is idle, and bit[7] is 0.  
  // Byte[1]  //humidity 
  // Byte[2]  //humidity 
  // Byte[3]  //humidity / temperature
  // Byte[4]  //temperature 
  // Byte[5]  //temperature 
  RetuData = (RetuData|Byte[1])<<8;
  RetuData = (RetuData|Byte[2])<<8;
  RetuData = (RetuData|Byte[3]);
  RetuData =RetuData >>4;
  ct[0] = float(RetuData)*100/1024/1024;          //humidity 
  RetuData = 0;
  RetuData = (RetuData|Byte[3])<<8;
  RetuData = (RetuData|Byte[4])<<8;
  RetuData = (RetuData|Byte[5]);
  RetuData = RetuData&0xfffff;
  ct[1] =float(RetuData)*200/1024/1024-50;        //temperature 
}


//CRC check, read AHT20 temperature and humidity data directly
void AHT20_Read_CTdata_crc(float *ct){
  byte  Byte[7] = {0,0,0,0,0,0,0};
  long  RetuData = 0;
  byte  cnt = 0;  
  byte  CTDATA[6]={0};           // For CRC pass arrays
  
  AHT20_SendAC();                // Send the AC command to AHT20
  delay(80);

  //Until bit[7] is 0, indicating idle state. If it is 1, indicating busy state
  while(((AHT20_Read_Status()&0x80)==0x80)){
    delay(2);
    if(cnt++>=100){
     break;
     }
  }
  Wire.requestFrom(addr_, 7);    // request 1 bytes from slave device
  cnt = 0;
  while(Wire.available()){       // slave may send less than requested  
    Byte[cnt] = Wire.read();     // receive a byte as character
	if (cnt<6)
		CTDATA[cnt] = Byte[cnt];
    cnt++;
  }

  // Byte[0]  //Status word: the state is 0x98, indicating busy state, and bit[7] is 1.  The state is 0x1C, or 0x0C, or 0x08 is idle, and bit[7] is 0.  
  // Byte[1]  //humidity 
  // Byte[2]  //humidity 
  // Byte[3]  //humidity / temperature
  // Byte[4]  //temperature 
  // Byte[5]  //temperature 
  // Byte[6]  //CRC data
  if(Calc_CRC8(CTDATA,6)==Byte[6]){
    RetuData = (RetuData|Byte[1])<<8;
    RetuData = (RetuData|Byte[2])<<8;
    RetuData = (RetuData|Byte[3]);
    RetuData =RetuData >>4;
    ct[0] = float(RetuData)*100/1024/1024;          //humidity 
    RetuData = 0;
    RetuData = (RetuData|Byte[3])<<8;
    RetuData = (RetuData|Byte[4])<<8;
    RetuData = (RetuData|Byte[5]);
    RetuData = RetuData&0xfffff;
    ct[1] =float(RetuData)*200/1024/1024-50;        //temperature 
  } 
  else{
    ct[0]=0x00;
    ct[1]=0x00;
  }
}


byte AHT20_Read_Status(void){
  byte stat;
  Wire.requestFrom(addr_, 1);     // request 1 bytes from slave device
  while(Wire.available()){        // slave may send less than requested  
    stat = Wire.read();           // receive a byte as character
  }
  return stat;
}

void Reset_REG(int reg){
  byte i = 0;
  int  Byte[3] = {0,0,0};
  
  Wire.beginTransmission(addr_);  // transmit to device 
  Wire.write(reg);                // sends value byte 
  Wire.write(0x00);
  Wire.write(0x00);
  Wire.endTransmission();         // stop transmitting

  delay(5);
  Wire.requestFrom(addr_, 3);     // request 3 bytes from slave device
  while(Wire.available()){        // slave may send less than requested  
    Byte[i] = Wire.read();        // receive a byte as character
    i++;
  }

  delay(10);
  Wire.beginTransmission(addr_);  // transmit to device 
  Wire.write(0xb0|reg);           // sends value byte 
  Wire.write(Byte[1]);
  Wire.write(Byte[2]);
  Wire.endTransmission();         // stop transmitting  
}


void AHT20_Start_Init(void){
  Reset_REG(0x1b); 
  Reset_REG(0x1c);
  Reset_REG(0x1e);
}


void setup() {
  // It takes 100 to 500ms for the chip to be ready after it is powered on. 500ms is recommended
  delay(500);           
  Wire.begin();               // join i2c bus (address optional for master)
  Serial.begin(9600);         // start serial for output

  // When the chip is powered on for the first time, the system reads the status word by sending 0x71 and determines whether the status word is 0x18. 
  // If it is not 0x18, the register is initialized.  
  if((AHT20_Read_Status() & 0x18) != 0x18){
    AHT20_Start_Init();       // Reinitialize the register   
    delay(10);     
  }
}


void loop() {
  float HT_data[2];

  // You are advised to read the data every 1S 
  // Function prototype: MswHT.Read_CTdata(float *ct);  
  AHT20_Read_CTdata(HT_data);          // Read AHT20 temperature and humidity data directly without CRC check.
  
  // Function prototype: MswHT.Read_CTdata_crc(float *ct);
  //AHT20_Read_CTdata_crc(HT_data);    // After CRC verification, read temperature and humidity data of AHT20 
  delay(1000);
  
  Serial.print("Humidity = ");      
  Serial.println(HT_data[0]);
  Serial.print("Temperature = ");      
  Serial.println(HT_data[1]);
}

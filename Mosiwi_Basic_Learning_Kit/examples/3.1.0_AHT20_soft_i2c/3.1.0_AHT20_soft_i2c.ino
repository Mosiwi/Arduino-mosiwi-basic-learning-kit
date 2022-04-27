/*
 * Project: AHT20 soft i2c
 * Function: Read temperature and humidity
 * I2C communication protocol: https://www.nxp.com/docs/en/user-guide/UM10204.pdf
 * I2C communication protocol: http://www.i2c-bus.org/references/
 * Github: https://github.com/mosiwi
 * Wiki: http://wiki.mosiwi.com
 * Web: http://mosiwi.com
 * Engineer: Jalen
 * date: 2022-3-11
 */

// AHT20 device address
byte addr_ = 0x38 << 1;        

// AHT20 Register address
int reg1_ = 0x1b;
int reg2_ = 0x1c;
int reg3_ = 0x1e;

int ac_   = 0xac;
int ac_d1 = 0x33;
int ac_d2 = 0x00;

// Define i2C communication pins.
int sda   = A4;
int scl   = A5;

// Standard-mode delay time
#define ST 8   

////////////////////////////////////////////
// The I2C host sends the START signal
void I2C_Start(void){
  pinMode(sda, OUTPUT);
  digitalWrite(sda, HIGH);
  delayMicroseconds(ST);
  
  pinMode(scl, OUTPUT);
  digitalWrite(scl, HIGH);  
  delayMicroseconds(ST);
  
  digitalWrite(sda, LOW);
  delayMicroseconds(ST);
  
  digitalWrite(scl, LOW);
  delayMicroseconds(ST);
}

////////////////////////////////////////////
// The I2C host sends the STOP signal
void Stop_I2C(void){
  pinMode(sda, OUTPUT);
  digitalWrite(scl, LOW);
  
  digitalWrite(scl, HIGH);  
  delayMicroseconds(ST);
   
  digitalWrite(sda, HIGH);
  delayMicroseconds(ST);
}

////////////////////////////////////////////
// Check whether AHT20 has ACK signal.
byte Receive_ACK(void){
  byte CNT = 0;
  pinMode(scl, OUTPUT);
  digitalWrite(scl, LOW);
  delayMicroseconds(ST);
  
  digitalWrite(scl, HIGH);
  delayMicroseconds(ST);
     
  pinMode(sda, INPUT);  
  while((digitalRead(sda) == HIGH)  && (CNT < 100)){ 
    delayMicroseconds(10); 
    CNT++;
  }
  if(CNT >= 100){
	Serial.println("IIC slave device not ack!");
    return 0;
  } 
  
  digitalWrite(scl, LOW);
  delayMicroseconds(ST);  
  return 1;
}

////////////////////////////////////////////
//The host replies with an ACK signal
void Send_ACK(void){
  pinMode(scl, OUTPUT);
  digitalWrite(scl, LOW);
  delayMicroseconds(ST);
  
  pinMode(sda, OUTPUT);
  digitalWrite(sda, LOW);
  delayMicroseconds(ST);

  digitalWrite(scl, HIGH);  
  delayMicroseconds(ST); 
  
  digitalWrite(scl, LOW);
  delayMicroseconds(ST);

  pinMode(sda, INPUT);
  delayMicroseconds(ST);  
}

////////////////////////////////////////////
//The host does not reply to an ACK
void Send_NOT_ACK(void){
  pinMode(scl, OUTPUT);
  digitalWrite(scl, LOW);
  delayMicroseconds(ST);
  
  pinMode(sda, OUTPUT);
  digitalWrite(sda, HIGH);
  delayMicroseconds(ST); 
  
  digitalWrite(scl, HIGH);  
  delayMicroseconds(ST); 
  
  digitalWrite(scl, LOW);
  delayMicroseconds(ST);
  
  digitalWrite(sda, LOW);
  delayMicroseconds(ST);
}

////////////////////////////////////////////
// Write a byte to AHT20
void AHT20_WR_Byte(byte Byte){
  byte Data; 
  Data = Byte;
  pinMode(scl, OUTPUT);
  pinMode(sda, OUTPUT);
  
  for(byte N=0;N<8;N++){
	digitalWrite(scl, LOW);
	delayMicroseconds(ST/2);
    if((Data & 0x80) == 0x80)
      digitalWrite(sda, HIGH);
    else
      digitalWrite(sda, LOW);
	
    digitalWrite(scl, HIGH);
    delayMicroseconds(ST/2);
    Data = Data << 1;
  } 
  digitalWrite(scl, LOW);
  delayMicroseconds(ST);
  
  pinMode(sda, INPUT);
  delayMicroseconds(ST);
}

////////////////////////////////////////////
// Read one byte from AHT20
byte AHT20_RD_Byte(void){
  byte Byte = 0;
  pinMode(scl, OUTPUT);
  digitalWrite(scl, LOW); 
  pinMode(sda, INPUT); 
  delayMicroseconds(ST); 
  
  for(byte i=0;i<8;i++){ 
    digitalWrite(scl, HIGH);  
    delayMicroseconds(5); 
    if(digitalRead(sda) == HIGH) 
      Byte = (Byte<<1) | 1; 
    else
      Byte = Byte<<1;
    digitalWrite(scl, LOW);
    delayMicroseconds(5);   
  }  
  //pinMode(sda, INPUT); 
  //delayMicroseconds(ST); 
  return Byte;
}

////////////////////////////////////////////
//CRC：CRC8/MAXIM
//polynomial：X8+X5+X4+1
//Poly：0011 0001  0x31
//If I put the higher order in the back, it becomes: 1000 1100 0x8c
byte Calc_CRC8(byte *message,byte Num)
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

////////////////////////////////////////////
void AHT20_SendAC(void){
  I2C_Start();
  AHT20_WR_Byte(addr_);
  Receive_ACK();
  AHT20_WR_Byte(ac_);             //0xAC
  Receive_ACK();
  AHT20_WR_Byte(ac_d1);
  Receive_ACK();
  AHT20_WR_Byte(ac_d2);
  Receive_ACK();
  Stop_I2C();
}

////////////////////////////////////////////
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
  I2C_Start();
  AHT20_WR_Byte(addr_+1);
  Receive_ACK();
  Byte[0] = AHT20_RD_Byte(); //Status word: the state is 0x98, indicating busy state, and bit[7] is 1.  The state is 0x1C, or 0x0C, or 0x08 is idle, and bit[7] is 0.
  Send_ACK();
  Byte[1] = AHT20_RD_Byte(); //humidity 
  Send_ACK();
  Byte[2] = AHT20_RD_Byte(); //humidity 
  Send_ACK();
  Byte[3] = AHT20_RD_Byte(); //humidity / temperature
  Send_ACK();
  Byte[4] = AHT20_RD_Byte(); //temperature
  Send_ACK();
  Byte[5] = AHT20_RD_Byte(); //temperature
  Send_NOT_ACK();
  Stop_I2C();
  
  RetuData = (RetuData|Byte[1])<<8;
  RetuData = (RetuData|Byte[2])<<8;
  RetuData = (RetuData|Byte[3]);
  RetuData = RetuData >>4;
  ct[0] = float(RetuData)*100/1024/1024;           //humidity 
  RetuData = 0;
  RetuData = (RetuData|Byte[3])<<8;
  RetuData = (RetuData|Byte[4])<<8;
  RetuData = (RetuData|Byte[5]);
  RetuData = RetuData&0xfffff;
  ct[1] = float(RetuData)*200/1024/1024-50;        //temperature 
}

////////////////////////////////////////////
//No CRC check, read AHT20 temperature and humidity data directly
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
  I2C_Start();
  AHT20_WR_Byte(addr_+1);
  Receive_ACK();
  CTDATA[0] = Byte[0] = AHT20_RD_Byte(); //Status word: the state is 0x98, indicating busy state, and bit[7] is 1.  The state is 0x1C, or 0x0C, or 0x08 is idle, and bit[7] is 0.
  Send_ACK();
  CTDATA[1] = Byte[1] = AHT20_RD_Byte(); //humidity 
  Send_ACK();
  CTDATA[2] = Byte[2] = AHT20_RD_Byte(); //humidity 
  Send_ACK();
  CTDATA[3] = Byte[3] = AHT20_RD_Byte(); //humidity / temperature
  Send_ACK();
  CTDATA[4] = Byte[4] = AHT20_RD_Byte(); //temperature
  Send_ACK();
  CTDATA[5] = Byte[5] = AHT20_RD_Byte(); //temperature
  Send_ACK();
  Byte[6] = AHT20_RD_Byte(); //CRC data
  Send_NOT_ACK();
  Stop_I2C();

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

////////////////////////////////////////////
byte AHT20_Read_Status(void){
  byte stat;
  I2C_Start();
  AHT20_WR_Byte(addr_+1);    
  Receive_ACK(); 
  stat = AHT20_RD_Byte();
  Send_NOT_ACK();
  Stop_I2C();
  return stat;
}

////////////////////////////////////////////
void Reset_REG(int reg){
  byte i = 0;
  int  Byte[3] = {0,0,0};

  I2C_Start();
  AHT20_WR_Byte(addr_);
  Receive_ACK();
  AHT20_WR_Byte(reg);
  Receive_ACK();
  AHT20_WR_Byte(0x00);
  Receive_ACK();
  AHT20_WR_Byte(0x00);
  Receive_ACK();
  Stop_I2C();

  delay(5);
  I2C_Start();
  AHT20_WR_Byte(addr_+1);//
  Receive_ACK();
  Byte[0] = AHT20_RD_Byte();
  Send_ACK();
  Byte[1] = AHT20_RD_Byte();
  Send_ACK();
  Byte[2] = AHT20_RD_Byte();
  Send_NOT_ACK();
  Stop_I2C();
  
  delay(10);
  I2C_Start();
  AHT20_WR_Byte(addr_);
  Receive_ACK();
  AHT20_WR_Byte(0xB0|reg);
  Receive_ACK();
  AHT20_WR_Byte(Byte[1]);
  Receive_ACK();
  AHT20_WR_Byte(Byte[2]);
  Receive_ACK();
  Stop_I2C(); 
}

////////////////////////////////////////////
void AHT20_Start_Init(void){
  Reset_REG(0x1b); 
  Reset_REG(0x1c);
  Reset_REG(0x1e);
}

////////////////////////////////////////////
void setup() {
  // It takes 100 to 500ms for the chip to be ready after it is powered on. 500ms is recommended
  delay(500);           
  Serial.begin(9600);         // start serial for output

  // When the chip is powered on for the first time, the system reads the status word by sending 0x71 and determines whether the status word is 0x18. 
  // If it is not 0x18, the register is initialized.  
  if((AHT20_Read_Status() & 0x18) != 0x18){
    AHT20_Start_Init();       // Reinitialize the register   
    delay(10);     
  }
}

////////////////////////////////////////////
void loop() {
  float HT_data[2];

  // You are advised to read the data every 1S  
  AHT20_Read_CTdata(HT_data);          // Read AHT20 temperature and humidity data directly without CRC check.
  
  //AHT20_Read_CTdata_crc(HT_data);    // After CRC verification, read temperature and humidity data of AHT20 
  delay(1000);
  
  Serial.println(HT_data[0]);          //The humidity value      
  Serial.println(HT_data[1]);          //The temperature value 
}



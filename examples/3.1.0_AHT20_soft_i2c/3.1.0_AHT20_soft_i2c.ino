/*
 * Project: AHT20 soft i2c
 * Function: Read temperature and humidity
 * I2C communication protocol: http://wiki.mosiwi.com/images/c/c0/UM10204%28I2C_Bus_Specification_and_User_Manual_%29.pdf
 * I2C communication protocol: http://www.i2c-bus.org/references/
 * Github: https://github.com/mosiwi
 * Wiki: http://wiki.mosiwi.com
 * Web: http://mosiwi.com
 * Engineer: Jalen
 * date: 2022-3-11
 */

// Standard-mode delay time
#define ST 8 

// AHT20 device address
byte addr_ = 0x38 << 1;        

// AHT20 Register address
byte reg1_ = 0x1b;
byte reg2_ = 0x1c;
byte reg3_ = 0x1e;

byte ac_   = 0xac;
byte ac_d1 = 0x33;
byte ac_d2 = 0x00;

// Define i2C communication pins.
byte sda   = A4;
byte scl   = A5;  

////////////////////////////////////////////
// The I2C master sends the START signal
void Start_I2C(void){
  pinMode(scl, OUTPUT);
  pinMode(sda, OUTPUT);
  
  digitalWrite(scl, HIGH); 
  digitalWrite(sda, HIGH);
  delayMicroseconds(ST);

  digitalWrite(sda, LOW);
  delayMicroseconds(ST);
}

////////////////////////////////////////////
// The I2C master sends the STOP signal
void Stop_I2C(void){
  pinMode(scl, OUTPUT);
  digitalWrite(scl, LOW);
  delayMicroseconds(ST);
  
  pinMode(sda, OUTPUT);
  digitalWrite(sda, LOW); 
  delayMicroseconds(ST);
  
  digitalWrite(scl, HIGH);
  delayMicroseconds(ST); 
   
  digitalWrite(sda, HIGH);
  delayMicroseconds(ST);

  pinMode(scl, INPUT);
  pinMode(sda, INPUT);
}

////////////////////////////////////////////
// Check whether slave has ACK signal.
byte Slave_ACK(void){
  byte CNT = 0;
  pinMode(scl, OUTPUT);
  digitalWrite(scl, LOW);
  delayMicroseconds(ST);
  
  pinMode(sda, INPUT);
  delayMicroseconds(ST);
  digitalWrite(scl, HIGH);
  
  while((digitalRead(sda) == HIGH)  && (CNT < 100)){ 
    delayMicroseconds(10); 
    CNT++;
  }
  if(CNT >= 100){
	Serial.println("IIC slave device not ack!");
    return 0;
  } 
  return 1;
}

////////////////////////////////////////////
//The master replies with an ACK signal
void Master_ACK(void){
  pinMode(scl, OUTPUT);
  digitalWrite(scl, LOW);
  delayMicroseconds(ST);
  
  pinMode(sda, OUTPUT);
  digitalWrite(sda, LOW);
  delayMicroseconds(ST);

  digitalWrite(scl, HIGH);  
  delayMicroseconds(ST);  
}

////////////////////////////////////////////
//The master does not reply to an ACK
void Master_NACK(void){
  pinMode(scl, OUTPUT);
  digitalWrite(scl, LOW);
  delayMicroseconds(ST);
  
  pinMode(sda, OUTPUT);
  digitalWrite(sda, HIGH);
  delayMicroseconds(ST); 
  
  digitalWrite(scl, HIGH);  
  delayMicroseconds(ST); 
}

////////////////////////////////////////////
// Write a byte to slave
void Master_WR_Byte(byte Byte){
  byte Data = Byte;
  pinMode(scl, OUTPUT);
  digitalWrite(scl, LOW);
  pinMode(sda, OUTPUT);
  
  for(byte N=0;N<8;N++){
	digitalWrite(scl, LOW);
	delayMicroseconds(ST);
    if((Data & 0x80) == 0x80)
      digitalWrite(sda, HIGH);
    else
      digitalWrite(sda, LOW);
    delayMicroseconds(ST);
    digitalWrite(scl, HIGH);
    delayMicroseconds(ST);
    Data = Data << 1;
  } 
}

////////////////////////////////////////////
// Read one byte from slave
byte Master_RD_Byte(void){
  byte Byte = 0;
  pinMode(scl, OUTPUT);
  digitalWrite(scl, LOW); 
  pinMode(sda, INPUT); 
  delayMicroseconds(ST); 
  
  for(byte i=0;i<8;i++){ 
    digitalWrite(scl, HIGH);  
    delayMicroseconds(ST); 
    if(digitalRead(sda) == HIGH) 
      Byte = (Byte<<1) | 1; 
    else
      Byte = Byte<<1;
    digitalWrite(scl, LOW);
    delayMicroseconds(ST);   
  }  
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
void SendAC_to_AHT20_(void){
  Start_I2C();
  Master_WR_Byte(addr_);
  Slave_ACK();
  Master_WR_Byte(ac_);             //0xAC
  Slave_ACK();
  Master_WR_Byte(ac_d1);
  Slave_ACK();
  Master_WR_Byte(ac_d2);
  Slave_ACK();
  Stop_I2C();
}

////////////////////////////////////////////
//No CRC check, read AHT20 temperature and humidity data directly
void Read_CTdata_From_AHT20(float *ct){
  byte  Byte[6] = {0,0,0,0,0,0};
  long  RetuData = 0;
  byte  cnt = 0;  

  SendAC_to_AHT20_();                // Send the AC command to AHT20
  delay(80);

  //Until bit[7] is 0, indicating idle state. If it is 1, indicating busy state
  while(((Read_AHT20_Status()&0x80)==0x80)){
    delay(2);
    if(cnt++>=100){
     break;
     }
  }
  Start_I2C();
  Master_WR_Byte(addr_+1);
  Slave_ACK();
  Byte[0] = Master_RD_Byte(); //Status word: the state is 0x98, indicating busy state, and bit[7] is 1.  The state is 0x1C, or 0x0C, or 0x08 is idle, and bit[7] is 0.
  Master_ACK();
  Byte[1] = Master_RD_Byte(); //humidity 
  Master_ACK();
  Byte[2] = Master_RD_Byte(); //humidity 
  Master_ACK();
  Byte[3] = Master_RD_Byte(); //humidity / temperature
  Master_ACK();
  Byte[4] = Master_RD_Byte(); //temperature
  Master_ACK();
  Byte[5] = Master_RD_Byte(); //temperature
  Master_NACK();
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
//CRC check, read AHT20 temperature and humidity data directly
void Read_CTdata_From_AHT20_crc(float *ct){
  byte  Byte[7] = {0,0,0,0,0,0,0};
  long  RetuData = 0;
  byte  cnt = 0;  
  byte  CTDATA[6]={0};           // For CRC pass arrays
  
  SendAC_to_AHT20_();                // Send the AC command to AHT20
  delay(80);

  //Until bit[7] is 0, indicating idle state. If it is 1, indicating busy state
  while(((Read_AHT20_Status()&0x80)==0x80)){
    delay(2);
    if(cnt++>=100){
     break;
     }
  }
  Start_I2C();
  Master_WR_Byte(addr_+1);
  Slave_ACK();
  CTDATA[0] = Byte[0] = Master_RD_Byte(); //Status word: the state is 0x98, indicating busy state, and bit[7] is 1.  The state is 0x1C, or 0x0C, or 0x08 is idle, and bit[7] is 0.
  Master_ACK();
  CTDATA[1] = Byte[1] = Master_RD_Byte(); //humidity 
  Master_ACK();
  CTDATA[2] = Byte[2] = Master_RD_Byte(); //humidity 
  Master_ACK();
  CTDATA[3] = Byte[3] = Master_RD_Byte(); //humidity / temperature
  Master_ACK();
  CTDATA[4] = Byte[4] = Master_RD_Byte(); //temperature
  Master_ACK();
  CTDATA[5] = Byte[5] = Master_RD_Byte(); //temperature
  Master_ACK();
  Byte[6] = Master_RD_Byte(); //CRC data
  Master_NACK();
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
byte Read_AHT20_Status(void){
  byte stat;
  Start_I2C();
  Master_WR_Byte(addr_+1);    
  Slave_ACK(); 
  stat = Master_RD_Byte();
  Master_NACK();
  Stop_I2C();
  return stat;
}

////////////////////////////////////////////
void Reset_AHT20_REG(int reg){
  byte i = 0;
  int  Byte[3] = {0,0,0};

  Start_I2C();
  Master_WR_Byte(addr_);
  Slave_ACK();
  Master_WR_Byte(reg);
  Slave_ACK();
  Master_WR_Byte(0x00);
  Slave_ACK();
  Master_WR_Byte(0x00);
  Slave_ACK();
  Stop_I2C();

  delay(5);
  Start_I2C();
  Master_WR_Byte(addr_+1);
  Slave_ACK();
  Byte[0] = Master_RD_Byte();
  Master_ACK();
  Byte[1] = Master_RD_Byte();
  Master_ACK();
  Byte[2] = Master_RD_Byte();
  Master_NACK();
  Stop_I2C();
  
  delay(10);
  Start_I2C();
  Master_WR_Byte(addr_);
  Slave_ACK();
  Master_WR_Byte(0xB0|reg);
  Slave_ACK();
  Master_WR_Byte(Byte[1]);
  Slave_ACK();
  Master_WR_Byte(Byte[2]);
  Slave_ACK();
  Stop_I2C(); 
}

////////////////////////////////////////////
//Initialize registers 0x1B, 0x1C, and 0x1E
void Start_Init_AHT20(void){
  Reset_AHT20_REG(0x1b); 
  Reset_AHT20_REG(0x1c);
  Reset_AHT20_REG(0x1e);
}

////////////////////////////////////////////
void setup() {          
  Serial.begin(9600);         // start serial for output

  // It takes 100 to 500ms for the chip to be ready after it is powered on. 500ms is recommended
  delay(500); 
  // When the chip is powered on for the first time, the system reads the status word by sending 0x71 and determines whether the status word is 0x18. 
  // If it is not 0x18, the register is initialized.  
  if((Read_AHT20_Status() & 0x18) != 0x18){
    Start_Init_AHT20();       // Reinitialize the register   
    delay(10);     
  }
}

////////////////////////////////////////////
void loop() {
  float HT_data[2];

  // You are advised to read the data every 1S  
  //Read_CTdata_From_AHT20(HT_data);      // Read AHT20 temperature and humidity data directly without CRC check.
  Read_CTdata_From_AHT20_crc(HT_data);    // After CRC verification, read temperature and humidity data of AHT20 
  delay(1000);
 
  Serial.print("Humidity = ");  
  Serial.println(HT_data[0]);             //The humidity value 
  Serial.print("Temperature = ");   
  Serial.println(HT_data[1]);             //The temperature value 
}

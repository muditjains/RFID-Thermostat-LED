

#include <SPI.h>
#include <MFRC522.h>
#include "Wire.h"
#define RST_PIN         9          
#define SS_PIN          10         
const int MPU_ADDR = 0x68;
MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance

String tagUID = "2C 34 41 33";  // This is for the keyfob.
int16_t accelerometer_x, accelerometer_y, accelerometer_z; // variables for accelerometer raw data
int16_t gyro_x, gyro_y, gyro_z; // variables for gyro raw data
int16_t temperature; // variables for temperature data

void setup() {
  Serial.begin(9600);   // Initialize serial communications with the PC
  while (!Serial);    // Do nothing if no serial port is opened (added for Arduinos based on ATMEGA32U4)
  SPI.begin();      // Init SPI bus
  mfrc522.PCD_Init();   // Init MFRC522
  mfrc522.PCD_DumpVersionToSerial();  // Show details of PCD - MFRC522 Card Reader details
  Serial.println(F("Scan PICC to see UID, SAK, type, and data blocks..."));
  // pins setup
  pinMode(8,OUTPUT) ; 
  pinMode(7,OUTPUT) ; 
  pinMode(6,OUTPUT) ; 
  pinMode(5,OUTPUT) ; 
  pinMode(4,OUTPUT) ; 
  pinMode(3,OUTPUT) ; 
  pinMode(2,OUTPUT) ; 
  
  Wire.begin();
  Wire.beginTransmission(MPU_ADDR); // Begins a transmission to the I2C slave (GY-521 board)
  Wire.write(0x6B); // PWR_MGMT_1 register
  Wire.write(0); // set to zero (wakes up the MPU-6050)
  Wire.endTransmission(true);
}

void loop() {
  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent()) {
    return;
  }

  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) {
    return;
  }
 
  String tag = "";
  for (byte i = 0; i < mfrc522.uid.size; i++)
  {
    tag.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
    tag.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  tag.toUpperCase();
  //Checking the card
  if (tag.substring(1) == tagUID) //change here the UID of the card/cards that you want to give access
  {
      Serial.println(F("Granted"));
      digitalWrite(8,HIGH);
      delay(2000);
      digitalWrite(8,LOW);

       Wire.beginTransmission(MPU_ADDR);
       Wire.write(0x3B); // starting with register 0x3B (ACCEL_XOUT_H) [MPU-6000 and MPU-6050 Register Map and Descriptions Revision 4.2, p.40]
       Wire.endTransmission(false); // the parameter indicates that the Arduino will send a restart. As a result, the connection is kept active.
       Wire.requestFrom(MPU_ADDR, 7*2, true); // request a total of 7*2=14 registers


        accelerometer_x = Wire.read()<<8 | Wire.read(); // reading registers: 0x3B (ACCEL_XOUT_H) and 0x3C (ACCEL_XOUT_L)
        accelerometer_y = Wire.read()<<8 | Wire.read(); // reading registers: 0x3D (ACCEL_YOUT_H) and 0x3E (ACCEL_YOUT_L)
        accelerometer_z = Wire.read()<<8 | Wire.read(); // reading registers: 0x3F (ACCEL_ZOUT_H) and 0x40 (ACCEL_ZOUT_L)
        temperature = Wire.read()<<8 | Wire.read(); // reading registers: 0x41 (TEMP_OUT_H) and 0x42 (TEMP_OUT_L)
        gyro_x = Wire.read()<<8 | Wire.read(); // reading registers: 0x43 (GYRO_XOUT_H) and 0x44 (GYRO_XOUT_L)
        gyro_y = Wire.read()<<8 | Wire.read(); // reading registers: 0x45 (GYRO_YOUT_H) and 0x46 (GYRO_YOUT_L)
        gyro_z = Wire.read()<<8 | Wire.read(); // reading registers: 0x47 (GYRO_ZOUT_H) and 0x48 (GYRO_ZOUT_L)
        
        temperature = (1.8*(temperature/340.00+36.53)+32);
        Serial.println(temperature);
        if(temperature >= 70 && temperature < 72)
        {
          digitalWrite(6,HIGH);
        }
        if(temperature >= 72 && temperature < 73)
        {
          digitalWrite(6,HIGH);
          digitalWrite(5,HIGH);
        }

        if(temperature >= 73 && temperature < 74)
        {
          digitalWrite(6,HIGH);
          digitalWrite(5,HIGH);
          digitalWrite(4,HIGH);

        }
        if(temperature >= 74 && temperature < 75)
        {
          digitalWrite(6,HIGH);
          digitalWrite(5,HIGH);
          digitalWrite(4,HIGH);
          digitalWrite(3,HIGH);
        }
        if(temperature >= 75 && temperature < 80)
        {
          digitalWrite(6,HIGH);
          digitalWrite(5,HIGH);
          digitalWrite(4,HIGH);
          digitalWrite(3,HIGH);
          digitalWrite(2,HIGH);
        }


  }
  else
  {
          Serial.println(F("Denied"));


          // warning the user for the wrong card.
          digitalWrite(7,HIGH);
          delay(500);
          digitalWrite(7,LOW);
          delay(500);
          digitalWrite(7,HIGH);
          delay(500);
          digitalWrite(7,LOW);
          delay(500);
          digitalWrite(7,HIGH);
          delay(500);
          digitalWrite(7,LOW);
          delay(500);
          digitalWrite(7,HIGH);
          delay(500);
          digitalWrite(7,LOW);

          
          digitalWrite(6,LOW);
          digitalWrite(5,LOW);
          digitalWrite(4,LOW);
          digitalWrite(3,LOW);
          digitalWrite(2,LOW);
          
  }
  
}

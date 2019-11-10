#include"Arduino.h"
#include <Wire.h>
#include "sensor.h"


//const int MPU_addr=0x68;
//
//MPU6050::MPU6050()
//{
//    AcX=0;
//    AcY=0;
//    AcZ=0;
//    Te=0;
//    GyX=0;
//    GyY=0;
//    GyZ=0;
//    range=0;
//    //sensor =new VL53L1X();
//}
//MPU6050::~MPU6050()
//{
//  //delete sensor;
//}
//*************************************************************
//MPU6050初始化
//*************************************************************
//void MPU6050::Initial()
//{
////   Serial2.begin(9600);
////   Serial3.begin(9600);
//   Serial.begin(9600); 
//   
//   Wire.begin();
//   Wire.setClock(400000); // use 400 kHz I2C
//   Wire.beginTransmission(MPU_addr);
//   Wire.write(0x6B);
//   Wire.write(0);
//   Wire.endTransmission(true);
//
////   sensor->setTimeout(500);
////   if (!sensor->init())
////   {
////     Serial.println("Failed to detect and initialize sensor!");
////     while (1);
////   }
////  sensor->setDistanceMode(VL53L1X::Long);
////  sensor->setMeasurementTimingBudget(50000);
////  sensor->startContinuous(50);
//}
//*************************************************************
//MPU6050数据获取
//*************************************************************
//void MPU6050::DataGet()
//{
//    Wire.beginTransmission(MPU_addr);
//    Wire.write(0x3B);
//    Wire.endTransmission(false);
//    Wire.requestFrom(MPU_addr,14,true);
//    AcY = Wire.read() << 8 |Wire.read();
//    AcY = Wire.read() << 8 |Wire.read();
//    AcZ = Wire.read() << 8 |Wire.read();
//    Te =  Wire.read() << 8 |Wire.read();
//    GyX = Wire.read() << 8 |Wire.read();
//    GyY = Wire.read() << 8 |Wire.read();
//    GyZ = Wire.read() << 8 |Wire.read();
//
////    sensor->read();
////    range=sensor->ranging_data.range_mm;
//
////    String Information=Serial1.readString();
////    if(Information=="red")
////      ColorState="red";
////    else if(Information=="green")
////       ColorState="green";
////    else 
////        ColorState="None";
//      
//}
//*************************************************************
//MPU6050数据发送
//*************************************************************
//void MPU6050::DataSend()
//{
//   Serial.print("AcX =");Serial.print(AcX);Serial.print('\n');
//   Serial.print("AcY =");Serial.print(AcY);Serial.print('\n');
//   Serial.print("AcZ =");Serial.print(AcZ);Serial.print('\n');
//   Serial.print("GyX =");Serial.print(GyX);Serial.print('\n');
//   Serial.print("GyY =");Serial.print(GyY); Serial.print('\n');
//   Serial.print("GyZ =");Serial.print(GyZ);Serial.print('\n');
//}

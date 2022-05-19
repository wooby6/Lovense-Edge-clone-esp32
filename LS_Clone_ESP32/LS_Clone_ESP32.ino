#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <BLE2902.h>
#include <SPI.h>
#include "MAX17043.h"  //https://github.com/DFRobot/DFRobot_MAX17043
#include "Wire.h"

/*
 * Notes
MA_F = Motor A Forward CW
MA_B = Motor A Backward CCW - Not Used
MB_F = Motor B Forward CW
MB_B = Motor B Backward CCW - Not Used
MC_F = Motor C Forward CW
MC_B = Motor C Backward CCW

*/

MAX17043 batteryMonitor;

BLEServer* pServer = NULL;
BLECharacteristic* pTxCharacteristic = NULL;
BLECharacteristic* pRxCharacteristic = NULL;
String bleAddress = "0082059AD3BD"; // CONFIGURATION: < Use the real device BLE address here.
bool deviceConnected = false;
bool oldDeviceConnected = false;
uint32_t value = 0;

int vibration;
#define SERVICE_UUID           "50300001-0023-4bd4-bbd5-a6920e4c5653"
#define CHARACTERISTIC_RX_UUID "50300002-0023-4bd4-bbd5-a6920e4c5653"
#define CHARACTERISTIC_TX_UUID "50300003-0023-4bd4-bbd5-a6920e4c5653"

//CONFIG
#define Device_Type            "P:37:FFFFFFFFFFFF;" // < only Needed if using with Official App
#define Batch                  "220123;" //Date Created currently Set to my birthday day this year

//Vibration Motor A = All Lovense Have One
const int MA_PWM=D2;  
const int MA_DIR=D3;  
//Vibration Motor B = Edge 2 Exclusive
const int MB_PWM=D5;
const int MB_DIR=D6;
//Rotation Motor C = Nora Exclusive
const int MC_PWM=D7;
const int MC_DIR=D9;
int CW = true;  //inital Rotation Direction if true CW if false Anti-Clockwise
//END CONFIG



const int PWMFreq1 = 1000;
const int PWM1 = 0;
const int PWMResolution1 = 8;

const int PWMFreq2 = 1000;
const int PWM2 = 1;
const int PWMResolution2 = 8;

const int PWMFreq3 = 1000;
const int PWM3 = 2;
const int PWMResolution3 = 8;

void MA_F(int SpeedA)
{
     ledcWrite(PWM1,SpeedA);
     digitalWrite(MA_DIR,LOW);
  }

void MA_B(int SpeedA)
{
    int SpeedA2=255-SpeedA;
    ledcWrite(PWM1,SpeedA2);
    digitalWrite(MA_DIR,HIGH);
  }

void MB_F(int SpeedB)
{
     ledcWrite(PWM2,SpeedB);
     digitalWrite(MB_DIR,LOW);
  }

void MB_B(int SpeedB)
{
    int SpeedB2=255-SpeedB;
    ledcWrite(PWM2,SpeedB2);
    digitalWrite(MB_DIR,HIGH);
  }

void MC_F(int SpeedC)
{
     ledcWrite(PWM3,SpeedC);
     digitalWrite(MC_DIR,LOW);
  }

void MC_B(int SpeedC)
{
    int SpeedC2=255-SpeedC;
    ledcWrite(PWM3,SpeedC2);
    digitalWrite(MC_DIR,HIGH);
  }

void VB (int Speed1) //shorcut to vibrate both motors at same time
{
    MA_F(Speed1);
    MB_F(Speed1);
}

class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      deviceConnected = true;
      BLEDevice::startAdvertising();
    };

    void onDisconnect(BLEServer* pServer) {
      deviceConnected = false;
    }
};



class MySerialCallbacks: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic) {
      static uint8_t messageBuf[64];
      assert(pCharacteristic == pRxCharacteristic);
      std::string rxValue = pRxCharacteristic->getValue();
      
      if (rxValue.length() > 0) {
        Serial.println("*********");
        Serial.print("Received Value: ");
        for (int i = 0; i < rxValue.length(); i++)
          Serial.print(rxValue[i]);

        Serial.println();
        Serial.println("*********");
      }

// Common Comands
      if (rxValue == "DeviceType;") { 
        // See CONFIG ^top
        Serial.println("$Responding to Device Enquiry");
        memmove(messageBuf, Device_Type, 18);
        pTxCharacteristic->setValue(messageBuf, 18);
        pTxCharacteristic->notify();
      } else if (rxValue == "Battery;") {
        float stateOfCharge = batteryMonitor.getSoC();
        memmove(messageBuf, "stateOfCharge;", 3);
        pTxCharacteristic->setValue(messageBuf, 3);
        pTxCharacteristic->notify();
      } else if (rxValue == "PowerOff;") {
        // Actually Doing this will require making a custom PCB
        memmove(messageBuf, "OK;", 3);
        pTxCharacteristic->setValue(messageBuf, 3);
        pTxCharacteristic->notify();
      } else if (rxValue == "GetBatch;") {
        // See CONFIG ^top
        memmove(messageBuf, Batch, 3);
        pTxCharacteristic->setValue(messageBuf, 3);
        pTxCharacteristic->notify();
      } else if (rxValue.rfind("Status:", 0) == 0) {
        memmove(messageBuf, "2;", 2);
        pTxCharacteristic->setValue(messageBuf, 3);
        pTxCharacteristic->notify();

// Common Vibrate Comands
      } else if (rxValue == "Vibrate:0;") {
        memmove(messageBuf, "OK;", 3);
        pTxCharacteristic->setValue(messageBuf, 3);
        pTxCharacteristic->notify();
        VB(0);
      } else if (rxValue == "Vibrate:1;") {
        memmove(messageBuf, "OK;", 3);
        pTxCharacteristic->setValue(messageBuf, 3);
        pTxCharacteristic->notify();
        VB(13);
      } else if (rxValue == "Vibrate:2;") {
        memmove(messageBuf, "OK;", 3);
        pTxCharacteristic->setValue(messageBuf, 3);
        pTxCharacteristic->notify();
        VB(25);
      } else if (rxValue == "Vibrate:3;") {
        memmove(messageBuf, "OK;", 3);
        pTxCharacteristic->setValue(messageBuf, 3);
        pTxCharacteristic->notify();
        VB(38);
      } else if (rxValue == "Vibrate:4;") {
        memmove(messageBuf, "OK;", 3);
        pTxCharacteristic->setValue(messageBuf, 3);
        pTxCharacteristic->notify();
        VB(51);
      } else if (rxValue == "Vibrate:5;") {
        memmove(messageBuf, "OK;", 3);
        pTxCharacteristic->setValue(messageBuf, 3);
        pTxCharacteristic->notify();
        VB(64);
      } else if (rxValue == "Vibrate:6;") {
        memmove(messageBuf, "OK;", 3);
        pTxCharacteristic->setValue(messageBuf, 3);
        pTxCharacteristic->notify();
        VB(76);
      } else if (rxValue == "Vibrate:7;") {
        memmove(messageBuf, "OK;", 3);
        pTxCharacteristic->setValue(messageBuf, 3);
        pTxCharacteristic->notify();
        VB(89);
      } else if (rxValue == "Vibrate:8;") {
        memmove(messageBuf, "OK;", 3);
        pTxCharacteristic->setValue(messageBuf, 3);
        pTxCharacteristic->notify();
        VB(102);
      } else if (rxValue == "Vibrate:9;") {
        memmove(messageBuf, "OK;", 3);
        pTxCharacteristic->setValue(messageBuf, 3);
        pTxCharacteristic->notify();
        VB(115);
      } else if (rxValue == "Vibrate:10;") {
        memmove(messageBuf, "OK;", 3);
        pTxCharacteristic->setValue(messageBuf, 3);
        pTxCharacteristic->notify();
        VB(127);
      } else if (rxValue == "Vibrate:11;") {
        memmove(messageBuf, "OK;", 3);
        pTxCharacteristic->setValue(messageBuf, 3);
        pTxCharacteristic->notify();
        VB(140);
      } else if (rxValue == "Vibrate:12;") {
        memmove(messageBuf, "OK;", 3);
        pTxCharacteristic->setValue(messageBuf, 3);
        pTxCharacteristic->notify();
        VB(153);
      } else if (rxValue == "Vibrate:13;") {
        memmove(messageBuf, "OK;", 3);
        pTxCharacteristic->setValue(messageBuf, 3);
        pTxCharacteristic->notify();
        VB(166);
      } else if (rxValue == "Vibrate:14;") {
        memmove(messageBuf, "OK;", 3);
        pTxCharacteristic->setValue(messageBuf, 3);
        pTxCharacteristic->notify();
        VB(178);
      } else if (rxValue == "Vibrate:15;") {
        memmove(messageBuf, "OK;", 3);
        pTxCharacteristic->setValue(messageBuf, 3);
        pTxCharacteristic->notify();
        VB(191);
      } else if (rxValue == "Vibrate:16;") {
        memmove(messageBuf, "OK;", 3);
        pTxCharacteristic->setValue(messageBuf, 3);
        pTxCharacteristic->notify();
        VB(204);
      } else if (rxValue == "Vibrate:17;") {
        memmove(messageBuf, "OK;", 3);
        pTxCharacteristic->setValue(messageBuf, 3);
        pTxCharacteristic->notify();
        VB(217);
      } else if (rxValue == "Vibrate:18;") {
        memmove(messageBuf, "OK;", 3);
        pTxCharacteristic->setValue(messageBuf, 3);
        pTxCharacteristic->notify();
        VB(229);
      } else if (rxValue == "Vibrate:19;") {
        memmove(messageBuf, "OK;", 3);
        pTxCharacteristic->setValue(messageBuf, 3);
        pTxCharacteristic->notify();
        VB(242);
      } else if (rxValue == "Vibrate:20;") {
        memmove(messageBuf, "OK;", 3);
        pTxCharacteristic->setValue(messageBuf, 3);
        pTxCharacteristic->notify();
        VB(255);
// EDGE Vibrate Comands
      }  else if (rxValue == "Vibrate1:0;") {
        memmove(messageBuf, "OK;", 3);
        pTxCharacteristic->setValue(messageBuf, 3);
        pTxCharacteristic->notify();
        MA_F(0);
      } else if (rxValue == "Vibrate1:1;") {
        memmove(messageBuf, "OK;", 3);
        pTxCharacteristic->setValue(messageBuf, 3);
        pTxCharacteristic->notify();
        MA_F(13);
      } else if (rxValue == "Vibrate1:2;") {
        memmove(messageBuf, "OK;", 3);
        pTxCharacteristic->setValue(messageBuf, 3);
        pTxCharacteristic->notify();
        MA_F(25);
      } else if (rxValue == "Vibrate1:3;") {
        memmove(messageBuf, "OK;", 3);
        pTxCharacteristic->setValue(messageBuf, 3);
        pTxCharacteristic->notify();
        MA_F(38);
      } else if (rxValue == "Vibrate1:4;") {
        memmove(messageBuf, "OK;", 3);
        pTxCharacteristic->setValue(messageBuf, 3);
        pTxCharacteristic->notify();
        MA_F(51);
      } else if (rxValue == "Vibrate1:5;") {
        memmove(messageBuf, "OK;", 3);
        pTxCharacteristic->setValue(messageBuf, 3);
        pTxCharacteristic->notify();
        MA_F(64);
      } else if (rxValue == "Vibrate1:6;") {
        memmove(messageBuf, "OK;", 3);
        pTxCharacteristic->setValue(messageBuf, 3);
        pTxCharacteristic->notify();
        MA_F(76);
      } else if (rxValue == "Vibrate1:7;") {
        memmove(messageBuf, "OK;", 3);
        pTxCharacteristic->setValue(messageBuf, 3);
        pTxCharacteristic->notify();
        MA_F(89);
      } else if (rxValue == "Vibrate1:8;") {
        memmove(messageBuf, "OK;", 3);
        pTxCharacteristic->setValue(messageBuf, 3);
        pTxCharacteristic->notify();
        MA_F(102);
      } else if (rxValue == "Vibrate1:9;") {
        memmove(messageBuf, "OK;", 3);
        pTxCharacteristic->setValue(messageBuf, 3);
        pTxCharacteristic->notify();
        MA_F(115);
      } else if (rxValue == "Vibrate1:10;") {
        memmove(messageBuf, "OK;", 3);
        pTxCharacteristic->setValue(messageBuf, 3);
        pTxCharacteristic->notify();
        MA_F(127);
      } else if (rxValue == "Vibrate1:11;") {
        memmove(messageBuf, "OK;", 3);
        pTxCharacteristic->setValue(messageBuf, 3);
        pTxCharacteristic->notify();
        MA_F(140);
      } else if (rxValue == "Vibrate1:12;") {
        memmove(messageBuf, "OK;", 3);
        pTxCharacteristic->setValue(messageBuf, 3);
        pTxCharacteristic->notify();
        MA_F(153);
      } else if (rxValue == "Vibrate1:13;") {
        memmove(messageBuf, "OK;", 3);
        pTxCharacteristic->setValue(messageBuf, 3);
        pTxCharacteristic->notify();
        MA_F(166);
      } else if (rxValue == "Vibrate1:14;") {
        memmove(messageBuf, "OK;", 3);
        pTxCharacteristic->setValue(messageBuf, 3);
        pTxCharacteristic->notify();
        MA_F(178);
      } else if (rxValue == "Vibrate1:15;") {
        memmove(messageBuf, "OK;", 3);
        pTxCharacteristic->setValue(messageBuf, 3);
        pTxCharacteristic->notify();
        MA_F(191);
      } else if (rxValue == "Vibrate1:16;") {
        memmove(messageBuf, "OK;", 3);
        pTxCharacteristic->setValue(messageBuf, 3);
        pTxCharacteristic->notify();
        MA_F(204);
      } else if (rxValue == "Vibrate1:17;") {
        memmove(messageBuf, "OK;", 3);
        pTxCharacteristic->setValue(messageBuf, 3);
        pTxCharacteristic->notify();
        MA_F(217);
      } else if (rxValue == "Vibrate1:18;") {
        memmove(messageBuf, "OK;", 3);
        pTxCharacteristic->setValue(messageBuf, 3);
        pTxCharacteristic->notify();
        MA_F(229);
      } else if (rxValue == "Vibrate1:19;") {
        memmove(messageBuf, "OK;", 3);
        pTxCharacteristic->setValue(messageBuf, 3);
        pTxCharacteristic->notify();
        MA_F(242);
      } else if (rxValue == "Vibrate1:20;") {
        memmove(messageBuf, "OK;", 3);
        pTxCharacteristic->setValue(messageBuf, 3);
        pTxCharacteristic->notify();
        MA_F(255);
      }  else if (rxValue == "Vibrate2:0;") {
        memmove(messageBuf, "OK;", 3);
        pTxCharacteristic->setValue(messageBuf, 3);
        pTxCharacteristic->notify();
        MB_F(0);
      } else if (rxValue == "Vibrate2:1;") {
        memmove(messageBuf, "OK;", 3);
        pTxCharacteristic->setValue(messageBuf, 3);
        pTxCharacteristic->notify();
        MB_F(13);
      } else if (rxValue == "Vibrate2:2;") {
        memmove(messageBuf, "OK;", 3);
        pTxCharacteristic->setValue(messageBuf, 3);
        pTxCharacteristic->notify();
        MB_F(25);
      } else if (rxValue == "Vibrate2:3;") {
        memmove(messageBuf, "OK;", 3);
        pTxCharacteristic->setValue(messageBuf, 3);
        pTxCharacteristic->notify();
        MB_F(38);
      } else if (rxValue == "Vibrate2:4;") {
        memmove(messageBuf, "OK;", 3);
        pTxCharacteristic->setValue(messageBuf, 3);
        pTxCharacteristic->notify();
        MB_F(51);
      } else if (rxValue == "Vibrate2:5;") {
        memmove(messageBuf, "OK;", 3);
        pTxCharacteristic->setValue(messageBuf, 3);
        pTxCharacteristic->notify();
        MB_F(64);
      } else if (rxValue == "Vibrate2:6;") {
        memmove(messageBuf, "OK;", 3);
        pTxCharacteristic->setValue(messageBuf, 3);
        pTxCharacteristic->notify();
        MB_F(76);
      } else if (rxValue == "Vibrate2:7;") {
        memmove(messageBuf, "OK;", 3);
        pTxCharacteristic->setValue(messageBuf, 3);
        pTxCharacteristic->notify();
        MB_F(89);
      } else if (rxValue == "Vibrate2:8;") {
        memmove(messageBuf, "OK;", 3);
        pTxCharacteristic->setValue(messageBuf, 3);
        pTxCharacteristic->notify();
        MB_F(102);
      } else if (rxValue == "Vibrate2:9;") {
        memmove(messageBuf, "OK;", 3);
        pTxCharacteristic->setValue(messageBuf, 3);
        pTxCharacteristic->notify();
        MB_F(115);
      } else if (rxValue == "Vibrate2:10;") {
        memmove(messageBuf, "OK;", 3);
        pTxCharacteristic->setValue(messageBuf, 3);
        pTxCharacteristic->notify();
        MB_F(127);
      } else if (rxValue == "Vibrate2:11;") {
        memmove(messageBuf, "OK;", 3);
        pTxCharacteristic->setValue(messageBuf, 3);
        pTxCharacteristic->notify();
        MB_F(140);
      } else if (rxValue == "Vibrate2:12;") {
        memmove(messageBuf, "OK;", 3);
        pTxCharacteristic->setValue(messageBuf, 3);
        pTxCharacteristic->notify();
        MB_F(153);
      } else if (rxValue == "Vibrate2:13;") {
        memmove(messageBuf, "OK;", 3);
        pTxCharacteristic->setValue(messageBuf, 3);
        pTxCharacteristic->notify();
        MB_F(166);
      } else if (rxValue == "Vibrate2:14;") {
        memmove(messageBuf, "OK;", 3);
        pTxCharacteristic->setValue(messageBuf, 3);
        pTxCharacteristic->notify();
        MB_F(178);
      } else if (rxValue == "Vibrate2:15;") {
        memmove(messageBuf, "OK;", 3);
        pTxCharacteristic->setValue(messageBuf, 3);
        pTxCharacteristic->notify();
        MB_F(191);
      } else if (rxValue == "Vibrate2:16;") {
        memmove(messageBuf, "OK;", 3);
        pTxCharacteristic->setValue(messageBuf, 3);
        pTxCharacteristic->notify();
        MB_F(204);
      } else if (rxValue == "Vibrate2:17;") {
        memmove(messageBuf, "OK;", 3);
        pTxCharacteristic->setValue(messageBuf, 3);
        pTxCharacteristic->notify();
        MB_F(217);
      } else if (rxValue == "Vibrate2:18;") {
        memmove(messageBuf, "OK;", 3);
        pTxCharacteristic->setValue(messageBuf, 3);
        pTxCharacteristic->notify();
        MB_F(229);
      } else if (rxValue == "Vibrate2:19;") {
        memmove(messageBuf, "OK;", 3);
        pTxCharacteristic->setValue(messageBuf, 3);
        pTxCharacteristic->notify();
        MB_F(242);
      } else if (rxValue == "Vibrate2:20;") {
        memmove(messageBuf, "OK;", 3);
        pTxCharacteristic->setValue(messageBuf, 3);
        pTxCharacteristic->notify();
        MB_F(255);
// Nora Commands
      } else if (rxValue == "RotateChange;") {
        memmove(messageBuf, "OK;", 3);
        pTxCharacteristic->setValue(messageBuf, 3);
        pTxCharacteristic->notify();
          if ( CW == true ) {
            CW = false;
          } else if  ( CW == false ) {
            CW = true;
          }
      }  else if (rxValue == "Rotate:0;") {
        memmove(messageBuf, "OK;", 3);
        pTxCharacteristic->setValue(messageBuf, 3);
        pTxCharacteristic->notify();
        if ( CW == true ) {
           MC_F(0);
          } else if  ( CW == false ) {
            MC_B(0);
          }
      } else if (rxValue == "Rotate:1;") {
        memmove(messageBuf, "OK;", 3);
        pTxCharacteristic->setValue(messageBuf, 3);
        pTxCharacteristic->notify();
        if ( CW == true  ) {
           MC_F(13);
          } else if  ( CW == false  ) {
            MC_B(13);
          }
      } else if (rxValue == "Rotate:2;") {
        memmove(messageBuf, "OK;", 3);
        pTxCharacteristic->setValue(messageBuf, 3);
        pTxCharacteristic->notify();
        if ( CW == true ) {
           MC_F(25);
          } else if  ( CW == false  ) {
            MC_B(25);
          }
      } else if (rxValue == "Rotate:3;") {
        memmove(messageBuf, "OK;", 3);
        pTxCharacteristic->setValue(messageBuf, 3);
        pTxCharacteristic->notify();
        if ( CW == true ) {
           MC_F(38);
          } else if  ( CW == false ) {
            MC_B(38);
          }
      } else if (rxValue == "Rotate:4;") {
        memmove(messageBuf, "OK;", 3);
        pTxCharacteristic->setValue(messageBuf, 3);
        pTxCharacteristic->notify();
        if ( CW == true ) {
           MC_F(51);
          } else if  ( CW == false ) {
            MC_B(51);
          }
      } else if (rxValue == "Rotate:5;") {
        memmove(messageBuf, "OK;", 3);
        pTxCharacteristic->setValue(messageBuf, 3);
        pTxCharacteristic->notify();
        if ( CW == true ) {
           MC_F(64);
          } else if  ( CW == false ) {
            MC_B(64);
          }
      } else if (rxValue == "Rotate:6;") {
        memmove(messageBuf, "OK;", 3);
        pTxCharacteristic->setValue(messageBuf, 3);
        pTxCharacteristic->notify();
        if ( CW == true ) {
           MC_F(76);
          } else if  ( CW == false ) {
            MC_B(76);
          }
      } else if (rxValue == "Rotate:7;") {
        memmove(messageBuf, "OK;", 3);
        pTxCharacteristic->setValue(messageBuf, 3);
        pTxCharacteristic->notify();
        if ( CW == true ) {
           MC_F(89);
          } else if  ( CW == true ) {
            MC_B(89);
          }
      } else if (rxValue == "Rotate:8;") {
        memmove(messageBuf, "OK;", 3);
        pTxCharacteristic->setValue(messageBuf, 3);
        pTxCharacteristic->notify();
        if ( CW == true ) {
           MC_F(102);
          } else if  ( CW == false ) {
            MC_B(102);
          }
      } else if (rxValue == "Rotate:9;") {
        memmove(messageBuf, "OK;", 3);
        pTxCharacteristic->setValue(messageBuf, 3);
        pTxCharacteristic->notify();
        if ( CW == true ) {
           MC_F(115);
          } else if  ( CW == false ) {
            MC_B(115);
          }
      } else if (rxValue == "Rotate:10;") {
        memmove(messageBuf, "OK;", 3);
        pTxCharacteristic->setValue(messageBuf, 3);
        pTxCharacteristic->notify();
        if ( CW == true ) {
           MC_F(127);
          } else if  ( CW == false ) {
            MC_B(127);
          }
      } else if (rxValue == "Rotate:11;") {
        memmove(messageBuf, "OK;", 3);
        pTxCharacteristic->setValue(messageBuf, 3);
        pTxCharacteristic->notify();
        if ( CW == true ) {
           MC_F(140);
          } else if  ( CW == false ) {
            MC_B(140);
          }
      } else if (rxValue == "Rotate:12;") {
        memmove(messageBuf, "OK;", 3);
        pTxCharacteristic->setValue(messageBuf, 3);
        pTxCharacteristic->notify();
        if ( CW == true ) {
           MC_F(153);
          } else if  ( CW == false ) {
            MC_B(153);
          }
      } else if (rxValue == "Rotate:13;") {
        memmove(messageBuf, "OK;", 3);
        pTxCharacteristic->setValue(messageBuf, 3);
        pTxCharacteristic->notify();
        if ( CW == true ) {
           MC_F(166);
          } else if  ( CW == false ) {
            MC_B(166);
          }
      } else if (rxValue == "Rotate:14;") {
        memmove(messageBuf, "OK;", 3);
        pTxCharacteristic->setValue(messageBuf, 3);
        pTxCharacteristic->notify();
        if ( CW == true ) {
           MC_F(178);
          } else if  ( CW == false ) {
            MC_B(178);
          }
      } else if (rxValue == "Rotate:15;") {
        memmove(messageBuf, "OK;", 3);
        pTxCharacteristic->setValue(messageBuf, 3);
        pTxCharacteristic->notify();
        if (CW == true ) {
           MC_F(191);
          } else if  ( CW == false ) {
            MC_B(191);
          }
      } else if (rxValue == "Rotate:16;") {
        memmove(messageBuf, "OK;", 3);
        pTxCharacteristic->setValue(messageBuf, 3);
        pTxCharacteristic->notify();
        if ( CW == true ) {
           MC_F(204);
          } else if  ( CW == false ) {
            MC_B(204);
          }
      } else if (rxValue == "Rotate:17;") {
        memmove(messageBuf, "OK;", 3);
        pTxCharacteristic->setValue(messageBuf, 3);
        pTxCharacteristic->notify();
        if ( CW == true ) {
           MC_F(217);
          } else if  ( CW == false ) {
            MC_B(217);
          }
      } else if (rxValue == "Rotate:18;") {
        memmove(messageBuf, "OK;", 3);
        pTxCharacteristic->setValue(messageBuf, 3);
        pTxCharacteristic->notify();
        if ( CW == true ) {
           MC_F(229);
          } else if  ( CW == false ) {
            MC_B(229);
          }
      } else if (rxValue == "Rotate:19;") {
        memmove(messageBuf, "OK;", 3);
        pTxCharacteristic->setValue(messageBuf, 3);
        pTxCharacteristic->notify();
        if ( CW == true ) {
           MC_F(242);
          } else if  ( CW == false ) {
            MC_B(242);
          }
      } else if (rxValue == "Rotate:20;") {
        memmove(messageBuf, "OK;", 3);
        pTxCharacteristic->setValue(messageBuf, 3);
        pTxCharacteristic->notify();
        if ( CW == true ) {
           MC_F(255);
          } else if  ( CW == false ) {
            MC_B(255);
          }
      

      }else {
        Serial.println("$Unknown request");        
        memmove(messageBuf, "ERR;", 4);
        pTxCharacteristic->setValue(messageBuf, 4);
        pTxCharacteristic->notify();
      }
    }
};

void setup() {
  Serial.begin(115200);

  SPI.begin();

  ledcSetup(PWM1, PWMFreq1, PWMResolution1);
  ledcSetup(PWM2, PWMFreq2, PWMResolution2);
  ledcSetup(PWM3, PWMFreq3, PWMResolution3);
  ledcAttachPin(MA_PWM, PWM1);
  ledcAttachPin(MB_PWM, PWM2);
  ledcAttachPin(MC_PWM, PWM2);
  pinMode(MA_DIR, OUTPUT);
  pinMode(MB_DIR, OUTPUT);
  pinMode(MC_DIR, OUTPUT);

  batteryMonitor.reset();
  batteryMonitor.quickStart();
  
  // Create the BLE Device
  BLEDevice::init("LVS-"); // CONFIGURATION: The name doesn't actually matter, The app identifies it by the reported id.

  // Create the BLE Server
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  // Create the BLE Service
  BLEService *pService = pServer->createService(SERVICE_UUID);

  // Create a BLE Characteristics
  pTxCharacteristic = pService->createCharacteristic(
                      CHARACTERISTIC_TX_UUID,
                      BLECharacteristic::PROPERTY_NOTIFY
                    );
  pTxCharacteristic->addDescriptor(new BLE2902());

  pRxCharacteristic = pService->createCharacteristic(
                      CHARACTERISTIC_RX_UUID,
                      BLECharacteristic::PROPERTY_WRITE  |
                      BLECharacteristic::PROPERTY_WRITE_NR
                    );
  pRxCharacteristic->setCallbacks(new MySerialCallbacks());

  // Start the service
  pService->start();

  // Start advertising
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(false);
  pAdvertising->setMinPreferred(0x0);  // set value to 0x00 to not advertise this parameter
  BLEDevice::startAdvertising();
  Serial.println("Waiting a client connection to notify...");
}

void loop() {
    // disconnecting
    if (!deviceConnected && oldDeviceConnected) {
        delay(100); // give the bluetooth stack the chance to get things ready
        pServer->startAdvertising(); // restart advertising
        Serial.println("start advertising");
        oldDeviceConnected = deviceConnected;
    }
    // connecting
    if (deviceConnected && !oldDeviceConnected) {
        // do stuff here on connecting
        oldDeviceConnected = deviceConnected;
    }
}

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <BLE2902.h>
#include <SPI.h>
#include "MAX17043.h" // https://porrey.github.io/max1704x/
#include "Wire.h"

BLEServer* pServer = NULL;
BLECharacteristic* pTxCharacteristic = NULL;
BLECharacteristic* pRxCharacteristic = NULL;
//CONFIG
String bleAddress = "0082059AD3BD"; // CONFIGURATION: < Use the real device BLE address here.
#define Device_Type            "P:37:0082059AD3BD;" // < only Needed if using with Official App
#define Batch                  "220123;" //Date Created currently Set to my birthday day this year
#define btname                 "LVS-"  // CONFIGURATION: keep it as LVS-  
#define SERVICE_UUID           "50300001-0023-4bd4-bbd5-a6920e4c5653"
#define CHARACTERISTIC_RX_UUID "50300002-0023-4bd4-bbd5-a6920e4c5653"
#define CHARACTERISTIC_TX_UUID "50300003-0023-4bd4-bbd5-a6920e4c5653"
//pins
const int MA_PWM=0;  
const int MA_DIR=1;  
const int MB_PWM=4;
const int MB_DIR=5;
//END CONFIG

bool deviceConnected = false;
bool oldDeviceConnected = false;
uint32_t value = 0;
int vibration;
float stateOfCharge = 69;
const int PWMF = 1000;
const int PWMR = 8;
const int PWM1 = 0;
const int PWM2 = 1;

void V (int M)
{
    int Speed = round(12.75*vibration);
    if (M == 1) {
    ledcWrite(PWM1,Speed);
    digitalWrite(MA_DIR,LOW);
    } else if (M == 2){
    ledcWrite(PWM2,Speed);
    digitalWrite(MB_DIR,LOW);
    } else {
    ledcWrite(PWM1,Speed);
    digitalWrite(MA_DIR,LOW);
    ledcWrite(PWM2,Speed);
    digitalWrite(MB_DIR,LOW);
    }
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
      if (rxValue == "DeviceType;") { 
        // See CONFIG ^top
        Serial.println("$Responding to Device Enquiry");
        memmove(messageBuf, Device_Type, 18);
        pTxCharacteristic->setValue(messageBuf, 18);
        pTxCharacteristic->notify();
      } else if (rxValue == "Battery;") {
        memmove(messageBuf, "stateOfCharge;", 3);
        pTxCharacteristic->setValue(messageBuf, 3);
        pTxCharacteristic->notify();
      } else if (rxValue == "PowerOff;") {
        // Actually Doing this will require making a custom PCB
        memmove(messageBuf, "OK;", 3);
        pTxCharacteristic->setValue(messageBuf, 3);
        pTxCharacteristic->notify();
      } else if (rxValue == "GetBatch;") {
        memmove(messageBuf, Batch, 3);
        pTxCharacteristic->setValue(messageBuf, 3);
        pTxCharacteristic->notify();
      } else if (rxValue.rfind("Status:", 0) == 0) {
        memmove(messageBuf, "2;", 2);
        pTxCharacteristic->setValue(messageBuf, 3);
        pTxCharacteristic->notify();
      } else if (rxValue.rfind("Vibrate:",0) == 0) {
        memmove(messageBuf, "OK;", 3);
        pTxCharacteristic->setValue(messageBuf, 3);
        pTxCharacteristic->notify();
        vibration = std::atoi(rxValue.substr(8).c_str());
        V(3);
      }  else if (rxValue.rfind("Vibrate1:",0) == 0) {
        memmove(messageBuf, "OK;", 3);
        pTxCharacteristic->setValue(messageBuf, 3);
        pTxCharacteristic->notify();
        vibration = std::atoi(rxValue.substr(9).c_str());
        V(1);
      }  else if (rxValue.rfind("Vibrate2:",0) == 0) {
        memmove(messageBuf, "OK;", 3);
        pTxCharacteristic->setValue(messageBuf, 3);
        pTxCharacteristic->notify();
        vibration = std::atoi(rxValue.substr(9).c_str());
        V(2);
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

  ledcSetup(PWM1, PWMF, PWMR);
  ledcSetup(PWM2, PWMF, PWMR);
  ledcAttachPin(MA_PWM, PWM1);
  ledcAttachPin(MB_PWM, PWM2);
  pinMode(MA_DIR, OUTPUT);
  pinMode(MB_DIR, OUTPUT);

if (FuelGauge.begin())
  {
    Serial.println("Resetting device...");
    FuelGauge.reset();
    delay(250);
    Serial.println("Initiating quickstart mode...");
    FuelGauge.quickstart();
    delay(125);
    Serial.print("Percent:       "); Serial.print(FuelGauge.percent()); Serial.println("%");
  }
  else
  {
    Serial.println("The MAX17043 device was NOT found.\n");
    while (true);
  }
  
  BLEDevice::init(btname);
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());
  BLEService *pService = pServer->createService(SERVICE_UUID);
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
  pService->start();
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(false);
  pAdvertising->setMinPreferred(0x0);  // set value to 0x00 to not advertise this parameter
  BLEDevice::startAdvertising();
  Serial.println("Waiting a client connection to notify...");
}

void loop() {
    if (!deviceConnected && oldDeviceConnected) {
        delay(100); // give the bluetooth stack the chance to get things ready
        pServer->startAdvertising(); // restart advertising
        Serial.println("start advertising");
        oldDeviceConnected = deviceConnected;
    }
    if (deviceConnected && !oldDeviceConnected) {
        oldDeviceConnected = deviceConnected;
    }
    stateOfCharge = FuelGauge.percent();
}

/*
    Video: https://www.youtube.com/watch?v=oCMOYS71NIU
    Based on Neil Kolban example for IDF: https://github.com/nkolban/esp32-snippets/blob/master/cpp_utils/tests/BLE%20Tests/SampleNotify.cpp
    Ported to Arduino ESP32 by Evandro Copercini

   Create a BLE server that, once we receive a connection, will send periodic notifications.
   创建一个BLE服务器，一旦我们收到连接，将会周期性发送通知

   T使用步骤：
   1. 创建一个 BLE Server
   2. 创建一个 BLE Service
   3. 创建一个 BLE Characteristic
   4. 创建一个 BLE Descriptor
   5. 开始服务
   6. 开始广播


*/
#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include <iostream>
// #include "common.h"

uint8_t txValue = 0;                         //后面需要发送的值
BLEServer *pServer = NULL;                   // BLEServer指针 pServer
BLECharacteristic *pTxCharacteristic = NULL; // BLECharacteristic指针 pTxCharacteristic
bool deviceConnected = false;                //本次连接状态
bool oldDeviceConnected = false;             //上次连接状态d
// See the following for generating UUIDs: https://www.uuidgenerator.net/
#define SERVICE_UUID "12a59900-17cc-11ec-9621-0242ac130002" // UART service UUID
#define CHARACTERISTIC_UUID_RX "12a59e0a-17cc-11ec-9621-0242ac130002"
#define CHARACTERISTIC_UUID_TX "12a5a148-17cc-11ec-9621-0242ac130002"

char state = 0;
// std::string led_1 = "1";   // 红灯闪烁
// std::string led_16 = "16"; // 蓝灯常亮
// std::string led_4 = "4";   // 红灯常亮
// std::string led_13 = "13"; // 绿灯常亮
// std::string led_num;       //  变量
// std::string led_0 = "0";   //  关灯
int RGB_Blue = 32;
int RGB_RED = 4;
int RGB_GREEN = 13;
class MyServerCallbacks : public BLEServerCallbacks
{
  void onConnect(BLEServer *pServer)
  {
    deviceConnected = true;
  };

  void onDisconnect(BLEServer *pServer)
  {
    deviceConnected = false;
  }
};

class MyCallbacks : public BLECharacteristicCallbacks
{
  void onWrite(BLECharacteristic *pCharacteristic)
  {
    std::string rxValue = pCharacteristic->getValue(); //接收信息
    if (rxValue.length() > 0)
    { //向串口输出收到的值
      std::cout << rxValue;
      // 1 是闪烁  2 是常亮 0 是关闭
      std::string close_0 = "0,0"; //  关闭
      std::string close_1 = "0,1"; //  三个灯来回闪烁

      std::string red_1 = "4,1"; // 红灯闪烁
      std::string red_2 = "4,2"; // 红灯常亮

      std::string green_1 = "13,1"; // 绿灯闪烁
      std::string green_2 = "13,2"; // 绿灯常亮

      std::string blue_1 = "32,1"; // 蓝灯闪烁
      std::string blue_2 = "32,2"; // 蓝灯常亮

      if (rxValue == close_0)
      {
        digitalWrite(RGB_RED, LOW);  // turn the LED off by making the voltage LOW
        digitalWrite(RGB_Blue, LOW); // turn the LED off by making the voltage LOW
        digitalWrite(RGB_GREEN, LOW);
      }

      if (rxValue == close_1)
      {
        pinMode(RGB_RED, OUTPUT);
        digitalWrite(RGB_RED, HIGH); // turn the LED on (HIGH is the voltage level)
        delay(1000);                 // wait for a second
        digitalWrite(RGB_RED, LOW);  // turn the LED off by making the voltage LOW
        delay(1000);
        pinMode(RGB_Blue, OUTPUT);
        digitalWrite(RGB_Blue, HIGH); // turn the LED on (HIGH is the voltage level)
        delay(1000);                  // wait for a second
        digitalWrite(RGB_Blue, LOW);  // turn the LED off by making the voltage LOW
        delay(1000);
        pinMode(RGB_GREEN, OUTPUT);
        digitalWrite(RGB_GREEN, HIGH); // turn the LED on (HIGH is the voltage level)
        delay(1000);                   // wait for a second
        digitalWrite(RGB_GREEN, LOW);  // turn the LED off by making the voltage LOW
        delay(1000);
      }

      if (rxValue == red_1)
      {
        digitalWrite(RGB_GREEN, LOW);
        digitalWrite(RGB_Blue, LOW);
        pinMode(RGB_RED, OUTPUT);
        digitalWrite(T4, HIGH);     // turn the LED on (HIGH is the voltage level)
        delay(1000);                // wait for a second
        digitalWrite(RGB_RED, LOW); // turn the LED off by making the voltage LOW
        delay(1000);
      }
      if (rxValue == red_2)
      {
        digitalWrite(RGB_GREEN, LOW);
        digitalWrite(RGB_Blue, LOW);
        pinMode(RGB_RED, OUTPUT);
        digitalWrite(RGB_RED, HIGH); // turn the LED on (HIGH is the voltage level)
        delay(1000);                 // wait for a second
      }

      if (rxValue == green_1)
      {
        digitalWrite(RGB_RED, LOW);
        digitalWrite(RGB_Blue, LOW);
        pinMode(RGB_GREEN, OUTPUT);
        digitalWrite(RGB_GREEN, HIGH); // turn the LED on (HIGH is the voltage level)
        delay(1000);                   // wait for a second
        digitalWrite(RGB_GREEN, LOW);  // turn the LED off by making the voltage LOW
        delay(1000);
      }
      if (rxValue == green_2)
      {
        digitalWrite(RGB_RED, LOW);
        digitalWrite(RGB_Blue, LOW);
        pinMode(RGB_GREEN, OUTPUT);
        digitalWrite(RGB_GREEN, HIGH); // turn the LED on (HIGH is the voltage level)
        delay(1000);                   // wait for a second
      }

      if (rxValue == blue_1)
      {
        digitalWrite(RGB_RED, LOW);
        digitalWrite(RGB_GREEN, LOW);
        pinMode(RGB_Blue, OUTPUT);
        digitalWrite(RGB_Blue, HIGH); // turn the LED on (HIGH is the voltage level)
        delay(1000);                  // wait for a second
        digitalWrite(RGB_Blue, LOW);  // turn the LED off by making the voltage LOW
        delay(1000);
      }
      if (rxValue == blue_2)
      {
        digitalWrite(RGB_RED, LOW);
        digitalWrite(RGB_GREEN, LOW);
        pinMode(RGB_Blue, OUTPUT);
        digitalWrite(RGB_Blue, HIGH); // turn the LED on (HIGH is the voltage level)
        delay(1000);                  // wait for a second
      }

      // for (int i = 0; i < rxValue.length(); i++)
      //   Serial.print(rxValue[i]);
      // Serial.println(rxValue);
      // Serial.println(rxValue);
      // led_num = rxValue; // 引脚
      // if (rxValue == led_1)
      // {
      //   pinMode(RGB_RED, OUTPUT);
      //   digitalWrite(RGB_RED, HIGH); // turn the LED on (HIGH is the voltage level)
      //   delay(1000);                 // wait for a second
      //   digitalWrite(RGB_RED, LOW);  // turn the LED off by making the voltage LOW
      //   delay(1000);
      // }
      // // if (state == '2')
      // // {
      // //   digitalWrite(RGB_RED, HIGH); // turn the LED on (HIGH is the voltage level)
      // // }

      // if (state == '0')
      // {
      //   // wait for a second
      //   digitalWrite(state, LOW); // turn the LED off by making the voltage LOW
      //   delay(1000);
      // }
      // // 绿灯
      // if (rxValue == led_13)
      // {

      //   digitalWrite(state, HIGH); // turn the LED on (HIGH is the voltage level)
      // }
      // // 红灯
      // if (rxValue == led_4)
      // {
      //   digitalWrite(state, HIGH); // turn the LED on (HIGH is the voltage level)
      // }
      // // 蓝灯
      // if (rxValue == led_16)
      // {
      //   digitalWrite(state, HIGH); // turn the LED on (HIGH is the voltage level)
      // }
    }
  }
};

void setup()
{
  Serial.begin(115200);

  // 创建一个 BLE 设备
  BLEDevice::init("SONDER"); //在这里面是ble的名称

  // 创建一个 BLE 服务
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks()); //设置回调
  BLEService *pService = pServer->createService(SERVICE_UUID);

  // 创建一个 BLE 特征
  pTxCharacteristic = pService->createCharacteristic(CHARACTERISTIC_UUID_TX, BLECharacteristic::PROPERTY_NOTIFY);
  pTxCharacteristic->addDescriptor(new BLE2902());
  BLECharacteristic *pRxCharacteristic = pService->createCharacteristic(CHARACTERISTIC_UUID_RX, BLECharacteristic::PROPERTY_WRITE);
  pRxCharacteristic->setCallbacks(new MyCallbacks()); //设置回调

  pService->start();                  // 开始服务
  pServer->getAdvertising()->start(); // 开始广播
  Serial.println(" 等待一个客户端连接，且发送通知... ");
}

void loop()
{
  // deviceConnected 已连接
  if (deviceConnected)
  {
    // pTxCharacteristic->setValue(&txValue, 1); // 设置要发送的值为1
    pTxCharacteristic->notify(); // 广播
    // txValue++;                                // 指针数值自加1
    delay(2000); // 如果有太多包要发送，蓝牙会堵塞
  }

  // disconnecting  断开连接
  if (!deviceConnected && oldDeviceConnected)
  {
    delay(500);                  // 留时间给蓝牙缓冲
    pServer->startAdvertising(); // 重新广播
    Serial.println(" 开始广播 ");
    oldDeviceConnected = deviceConnected;
  }

  // connecting  正在连接
  if (deviceConnected && !oldDeviceConnected)
  {
    // do stuff here on connecting
    oldDeviceConnected = deviceConnected;
  }
}

// #include <Arduino.h>
// #include <BluetoothSerial.h>
// // #include <SPI.h>
// // #include <TFT_eSPI.h>
// BluetoothSerial SerialBT;

// int state = 0;
// int RGB_Blue = 2;
// int RGB_RED = 4;
// int RGB_GREEN = 15;

// void setup()
// {
//   // 由于太紧张和不熟悉以至于忘记开始的串口通信波特率设置了。代码为"Serial.begin(115200);"
//   Serial.begin(115200);
//   // BLEDevice::init("ESP32BLE");
//   SerialBT.begin("SONDER_xiaomi"); // 如果没有参数传入则默认是蓝牙名称是: "ESP32"
//   SerialBT.setPin("1234");         // 蓝牙连接的配对码
//   Serial.printf("BT initial ok and ready to pair. \r\n");
//   // pinMode(RGB_RED, OUTPUT);
// }

// void loop()
// {
// }
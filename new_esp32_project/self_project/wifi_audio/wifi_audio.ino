//--------引入库-----------
#include <Arduino.h>
#include <WiFi.h>
#include "AudioSampler.h"
//--------定义常量---------
#define OPEN_DEBUG true //正式运行的时候请设置为false
#define SSID "NJUPT-CNCC"
#define PASSWORD "CX123456"
#define SERVER_IP "192.168.179.31"
#define SERVER_PORT 8888
#define uS_TO_S_FACTOR 1000000ULL


//#define TIME_TO_SLEEP  600        //睡眠时间10分钟，到时会醒来检查一次，还是没电会继续睡
//#define MUTEPIN 12


#define TOUCH_THRESHOLD 40        //触摸灵敏度阈值，越大越灵敏
#define STATUSPIN 18


char server_ip_last[15]="192.168.179.31";
char character = '\n'; //终止字符
int numdata=0;
char buffer[15];


//-------定义变量----------
volatile bool wifiConnected = false;
volatile bool mute = false;
volatile int statusLedState = LOW;
volatile unsigned long sinceLastTouch = 0;
volatile bool inited = false;

AudioSampler *audioSampler = NULL;



/**
   发送缓冲数组到服务器方法
   @param param
*/
void transmitTask(void *param) {
  AudioSampler *audioSampler = (AudioSampler *)param;
  //socket连接服务器
  WiFiClient *wifiClient = new WiFiClient();
  while (!wifiClient->connect(server_ip_last, SERVER_PORT)) {
     delay(100);
    Serial.println("IP maybe wrong!Enter a new IP(end with \\n)");
         if (Serial.available() > 0) {
    delay(100); // 等待数据传完
      numdata =Serial.readBytesUntil(character,buffer,15);
      Serial.println("Host IP changes now!");
      Serial.println(buffer);  
        for (int i = 0; i < 15; ++i) {
        server_ip_last[i] = buffer[i];
    }
     }
  }
  wifiClient->setNoDelay(true);
  const TickType_t xMaxBlockTime = pdMS_TO_TICKS(100);
  unsigned long startTime;
  unsigned long endTime;
  while (true) {
    // 等待队列通知
    uint32_t ulNotificationValue = ulTaskNotifyTake(pdTRUE, xMaxBlockTime);
    if (ulNotificationValue > 0) {
      //wifi连接上同时未静音才发送数据
      if (wifiConnected ) {
        int sendNum = wifiClient->write((uint8_t *)audioSampler->getTransmitBuffer(), audioSampler->getTransmitPackageSize());
      } else {
        //未连接时情况tcp缓存
        wifiClient->flush();

      }
    }
  }
}
void wifiEvent(WiFiEvent_t event) {
  switch (event) {
    case SYSTEM_EVENT_STA_CONNECTED:
      wifiConnected = true;
      //led亮起
      digitalWrite(STATUSPIN, HIGH);
      break;
    case SYSTEM_EVENT_STA_DISCONNECTED:
      //回调会多次执行，所以要判断一下
      if (inited) {
        wifiConnected = false;
        digitalWrite(STATUSPIN, LOW);
        ESP.restart();
      }
      break;
    default: break;
  }
}
void setup() {
  //状态led初始化

  pinMode(STATUSPIN, OUTPUT);
  digitalWrite(STATUSPIN, LOW);


  //是否开启debug
  if (OPEN_DEBUG) {
    Serial.begin(115200);
  }

  //初始化并连接WiFi
  WiFi.mode(WIFI_STA);
  WiFi.onEvent(wifiEvent);
  WiFi.begin(SSID, PASSWORD);

  //server_ip_last="192.168.179.31";
  //循环等待连接上WiFi
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    delay(500);
    if (statusLedState == LOW) {
      statusLedState = HIGH;
    } else {
      statusLedState = LOW;
    }
    digitalWrite(STATUSPIN, statusLedState);
    Serial.println(statusLedState);
  }


  //创建取样器对象
  audioSampler = new AudioSampler();
  //创建发送句柄
  TaskHandle_t transmitHandle;
  xTaskCreate(transmitTask, "transmitTask", 10240, audioSampler, 1,
              &transmitHandle);
  //启动采样
  audioSampler->start(2048, transmitHandle);

  //初始化完将初始化状态置为true
  inited = true;
}

void loop() {}

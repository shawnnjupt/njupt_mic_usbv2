//
// Created by BLab on 2021/10/5.
//

#include <Arduino.h>
#include "driver/i2s.h"
#include "AudioSampler.h"

void AudioSampler::processData(int16_t *adcData, size_t bytesRead) {
  for (int i = 0; i < bytesRead / 2; i++) {
    addSingleData(adcData[i]);
  }
}

void AudioSampler::addSingleData(int16_t singleData) {
  this->currentAudioBuffer[this->bufferPointer++] = singleData;
  if (this->bufferPointer == this->transmitPackageSize) {
    //过滤器处理

    //如果采样缓冲池满了就交换
    std::swap(this->currentAudioBuffer, this->transmitAudioBuffer);
    this->bufferPointer = 0;
    xTaskNotify(this->transmitHandle, 1, eIncrement);
  }
}
void SamplerTask(void *param) {
  AudioSampler *audioSampler = (AudioSampler *)param;
  while (true) {
    // 等待队列中有数据再处理
    // i2s_event_t event;
    // if (xQueueReceive(audioSampler->i2sQueue, &event, portMAX_DELAY) == pdPASS) {
    //   if (event.type == I2S_EVENT_RX_DONE) {
         //size_t bytesRead = 0;
          int16_t readData[1024];
          //int16_t testData;
          int i;
         for(i=0;i<1024;i++)
         {

        readData[i]=analogRead(ADCPIN)-415;
          // 处理原始数据

         }
                    vTaskDelay(1);
            audioSampler->processData(readData, 2048);


  }
}
void AudioSampler::start(int32_t transmitPackageSize, TaskHandle_t transmitHandle) {
  //this->i2SConfig = i2SConfig;
  //this->i2SPinConfig = i2SPinConfig;
  this->transmitPackageSize = transmitPackageSize / sizeof(int16_t);
  //保存通知句柄
  this->transmitHandle = transmitHandle;
  //分配buffer大小
  this->currentAudioBuffer = (int16_t *)malloc(transmitPackageSize);
  this->transmitAudioBuffer = (int16_t *)malloc(transmitPackageSize);
  //安装i2s驱动
  //i2s_driver_install(this->i2sPort, &i2SConfig, 4, &i2sQueue);
  //设置i2spin
  //i2s_set_pin(this->i2sPort, &i2SPinConfig);
  //在0核心中读取数据
  xTaskCreatePinnedToCore(SamplerTask, "AudioSampler", 10240, this, 1, NULL,
                          0);
}

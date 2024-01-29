# STM32F4--AUDIO--IV-Board  V1 2024-1-29

#### 渲染图

![image-20240129142832342](READMEmarkdown-img/README.assets/image-20240129142832342.png)

#### 参数：

移植库：

1、OLED显示库 u8g2

[BleakyTex/STM32F4_USB_MIC: A simple USB microphone with ADC oversampling using the STM32F407 MCU and MAX9814 microphone module (github.com)](https://github.com/BleakyTex/STM32F4_USB_MIC)

2、串口 终端shell库 letter shell

[letter-shell: 项目源码-letter-shell-NevermindZZT (gitee.com)](https://gitee.com/smallqing/letter-shell)

3、音频接口  [BleakyTex/STM32F4_USB_MIC: A simple USB microphone with ADC oversampling using the STM32F407 MCU and MAX9814 microphone module (github.com)](https://github.com/BleakyTex/STM32F4_USB_MIC)

#### 功能演示

接入stm32 audio 音频设备

![image-20240129143646259](READMEmarkdown-img/README.assets/image-20240129143646259.png)

音频波形显示（分辨率较低）

![image-20240129143809572](READMEmarkdown-img/README.assets/image-20240129143809572.png)

#### 调试软件

##### USB调试软件

Bus hound

![image-20240129145043074](READMEmarkdown-img/README.assets/image-20240129145043074.png)

##### 音频分析软件

audacity

![image-20240129155208922](READMEmarkdown-img/README.assets/image-20240129155208922.png)

#### 功能说明：

串口可调试

![image-20240129155657622](READMEmarkdown-img/README.assets/image-20240129155657622.png)

录音触发需要点击录音软件触发

#### 软件开发注意：

ADC触发为TIM3定时触发

![image-20240129160027790](READMEmarkdown-img/README.assets/image-20240129160027790.png)



USB设备符

![image-20240129160206439](READMEmarkdown-img/README.assets/image-20240129160206439.png)

过采样算法将12位ADC分辨率转换为14位

![image-20240129160453531](READMEmarkdown-img/README.assets/image-20240129160453531.png)

#### 参考：

usb-uac描述符参考

[UAC 简介及版本对比 - USB中文网 (usbzh.com)](https://www.usbzh.com/article/detail-185.html)


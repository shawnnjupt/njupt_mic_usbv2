# ESP32-AUDIO-Board  2024-5-29

主要功能：ESP32 ADC采用之后 联网传输音频数据(freertos 双核)，PC上位机接受数据之后提供录音、音频时域、频域、语谱图分析功能

![image-20240529233956488](READMEmarkdown-img/README.assets/image-20240529233956488.png)

![image-20240529233558303](READMEmarkdown-img/README.assets/image-20240529233558303.png)

![image-20240529233724532](READMEmarkdown-img/README.assets/image-20240529233724532.png)

![image-20240529233733241](READMEmarkdown-img/README.assets/image-20240529233733241.png)

#### 1、文件夹功能

![image-20240529233155575](READMEmarkdown-img/README.assets/image-20240529233155575.png)

esp32_project为esp32工程文件夹，采用arduino IDE设计

desktop_host为上位机文件夹，采用python设计，主要使用pyqt5库

handware为硬件文件夹，采用立创EDA设计

#### 2、硬件设计

![image-20240529234016043](READMEmarkdown-img/README.assets/image-20240529234016043.png)



![image-20240529234036734](READMEmarkdown-img/README.assets/image-20240529234036734.png)

#### 3、单片机程序设计

参考[BearLaboratory/wifiaudio-tx-hardware: WiFi无线麦克风硬件pcb设计 (github.com)](https://github.com/BearLaboratory/wifiaudio-tx-hardware)

使用freertos开两个线程对应ESP32两个核心，默认连接IP地址 "192.168.179.31"，网络端口 8888

cpu1:网络连接

![image-20240529234335153](READMEmarkdown-img/README.assets/image-20240529234335153.png)

cpu0:采样

![image-20240529234356842](READMEmarkdown-img/README.assets/image-20240529234356842.png)

参数更改:

网络名称：

网络密码：

主机IP地址：如果连接不上可以通过串口更改IP地址，会有提示

![image-20240529234628775](READMEmarkdown-img/README.assets/image-20240529234628775.png)

#### 4、上位机结构

使用pyqt5开发

默认获取IPv4地址，如果不对可以通过文本框更改，运用了正则表达式，不怕乱改

正常选择录音秒数之后可以开始录音，之后录音文件会出现在当前路径下，并且可以生成对应的波形图

![image-20240529234936107](READMEmarkdown-img/README.assets/image-20240529234936107.png)

#### 5、Citation
If you get inspired by our work, or otherwise find our work valuable, please cite our paper:

```23
@article{optocoupler,
  title={Highly integrated optocoupler based on monolithic III-nitride diodes for on-chip data transfer},
  author={Fan Shi,Chengxiang Jiang,Xiao Cong,Jiaqi Yin,Jiawei Shi,Jiabin Yan,Hongbo Zhu,Yongjin Wang},
  journal={Optics & Laser Technology},
  year={2025}
}
```

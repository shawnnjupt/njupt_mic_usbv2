# -*- coding:utf-8 -*-
import wave
import struct
import numpy as np
import matplotlib.pyplot as plt
def oscillogram_spectrum(audio_path):
    """
    画出音频文件audio_path的声波图和频谱图
    :param audio_path:音频文件路径
    :return:
    """
    # 读取wav文件
    filename = audio_path
    wavefile = wave.open(filename, 'r')  # open for writing
    # 读取wav文件的四种信息的函数。期中numframes表示一共读取了几个frames。
    nchannels = wavefile.getnchannels()
    sample_width = wavefile.getsampwidth()
    framerate = wavefile.getframerate()
    numframes = wavefile.getnframes()
    print("channel", nchannels)
    print("sample_width", sample_width)
    print("framerate", framerate)
    print("numframes", numframes)
    # 建一个y的数列，用来保存后面读的每个frame的amplitude。
    y = np.zeros(numframes)
    # for循环，readframe(1)每次读一个frame，取其前两位，是左声道的信息。右声道就是后两位啦。
    # unpack是struct里的一个函数，简单说来就是把＃packed的string转换成原来的数据，无论是什么样的数据都返回一个tuple。这里返回的是长度为一的一个
    # tuple，所以我们取它的第零位。
    for i in range(numframes):
        val = wavefile.readframes(1)
        left = val[0:2]
        # right = val[2:4]
        v = struct.unpack('h', left)[0]
        y[i] = v
    # framerate就是声音的采用率，文件初读取的值。
    Fs = framerate
    time = np.arange(0, numframes) * (1.0 / framerate)
    # print(time)
    # 显示时域图(波形图)
    plt.subplot(311)
    plt.plot(time, y)

    plt.subplot(312)
    fft_data = np.fft.rfft(y)
    fft_length = len(fft_data)
    fft_data = 20 * np.log10(np.abs(fft_data) / fft_length * 2)
    plt.plot(np.linspace(0,Fs/2,len(fft_data)), fft_data)

    # 显示频域图(频谱图)
    plt.subplot(313)
    plt.specgram(y, NFFT=1024, Fs=Fs, noverlap=900)
    plt.show()


if __name__ == '__main__':
    audio_path = '1khzsin.wav'
    oscillogram_spectrum(audio_path)

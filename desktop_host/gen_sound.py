import numpy as np
import soundfile as sf

# 设置采样率和持续时间
sample_rate = 44100  # 44.1kHz
duration = 5  # 5秒钟

# 生成时间轴
t = np.linspace(0, duration, int(sample_rate * duration), endpoint=False)

# 生成正弦波信号
frequency = 1000  # 440Hz
waveform = np.sin(2 * np.pi * frequency * t)

# 将信号保存为.wav文件
sf.write('1khzsin.wav', waveform, sample_rate)
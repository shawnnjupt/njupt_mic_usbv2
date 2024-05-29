
import sys
from audio_play import *
from PyQt5.Qt import *
import matplotlib
matplotlib.use("Qt5Agg")
import socket
import pyaudio
import wave
import re
import matplotlib.pyplot as plt
import numpy as np
import struct

#
# py_path, py_name = os.path.split(os.path.abspath(__file__))
# base_dir = pathlib.Path(py_path).absolute().parent  # sys.argv[0]
#
# if sys.path.count(base_dir) == 0:
#     sys.path.append(str(base_dir))
#
# from USRlib.raindrop_logo import *
# from USRlib.raindrop_recording import *
# from USRlib.raindrop_matTOwav import *
# from USRlib.raindrop_speech_recogntion import *
# from USRlib.raindrop_audio_localization import *
# from USRlib.raindrop_voiceprint_extration import *

def check_string(re_exp, str):
    res = re.search(re_exp, str)
    if res:
        return False
    else:
        return True



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

class MyMainWindow(QtWidgets.QMainWindow, Ui_MainWindow):
    def __init__(self, parent=None):
        super(MyMainWindow, self).__init__(parent)
        self.setupUi(self)
        self.rate=11000
        self.channel=1
        self.format=pyaudio.paInt16
        self.chunk = 1024
        # print(socket.gethostbyname_ex(socket.gethostname())[2][2])
        self.host="".join(socket.gethostbyname_ex(socket.gethostname())[2][2])
        # self.host= socket.gethostbyname(socket.gethostname())
        self.port = 8888
        self.socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.frames = []
        self.WAVE_OUTPUT_FILENAME = "test.wav"
        self.lineEdit.setText(self.host)
        self.pushButton_15.clicked.connect(self.button1_recording_clicked)
        self.pushButton_16.clicked.connect(self.button2_wav_ploting_clicked)





    def button1_recording_clicked(self):
        print("button2_recording_clicked")
        if(self.lineEdit.text()==""):
            self.label_15.setText("未填写IP地址")
        elif((check_string('(([1-9]?\d|1\d\d|2[0-4]\d|25[0-5])\.){3}([1-9]?\d|1\d\d|2[0-4]\d|25[0-5])',self.lineEdit.text()))):
            self.label_13.setText("IP地址格式错误")
        # elif (len(self.label_13.text())<10):
        #     self.label_15.setText("请先搜寻设备")
        else:
            self.host = self.lineEdit.text()
            self.label_13.setText("正在搜寻设备")
            print(self.host)
            self.socket.bind((self.host, self.port))
            self.socket.listen(1)
            self.conn, self.addr = self.socket.accept()
            self.label_13.setText("搜寻到设备，可以开始录音")
            self.frames=[]
            # self.conn, self.addr = self.socket.accept()
            self.pyaudio = pyaudio.PyAudio()
            self.stream = self.pyaudio.open(format=self.format, channels=self.channel, rate=self.rate, output=True, frames_per_buffer=self.chunk)
            for i in range(0, int(10.742 *self.lcdNumber_3.value() * 2.2)):
                self.data = self.conn.recv(1024)
                self.stream.write(self.data)
                self.frames.append(self.data)
            print("finished recording")
            self.label_15.setText("录音完成")
            self.stream.stop_stream()
            self.stream.close()
            # self.conn.close()

            self.pyaudio.terminate()
            self.waveFile = wave.open(self.WAVE_OUTPUT_FILENAME, 'wb')
            self.waveFile.setnchannels(self.channel)
            self.waveFile.setsampwidth(self.pyaudio.get_sample_size(self.format))
            self.waveFile.setframerate(self.rate)
            self.waveFile.writeframes(b''.join(self.frames))
            self.waveFile.close()

    def button2_wav_ploting_clicked(self):
        audio_path = 'test.wav'
        oscillogram_spectrum(audio_path)













app = QApplication(sys.argv)

demo=MyMainWindow()


demo.setWindowTitle("ESP32音频接收上位机")
demo.setWindowIcon(QIcon('1.ico'))
demo.show()

sys.exit(app.exec())
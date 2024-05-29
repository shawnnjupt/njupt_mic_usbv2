package pro.dengyi;

import javax.sound.sampled.*;
import java.io.BufferedInputStream;
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.net.InetAddress;
import java.net.ServerSocket;
import java.net.Socket;

/**
 * @author dengyi (email:dengyi@dengyi.pro)
 * @date 2021-10-06
 */
public class AudioServerV2 {


    public static void main(String[] args) throws IOException, LineUnavailableException {
        ServerSocket serverSocket = new ServerSocket(8888);
        //存储文件，不存在就创建，创建就追加
        File file = new File("D:\\files\\gitee\\njupt_mic_usbv2\\new_software\\self_project\\au.raw");
        FileOutputStream fileOutputStream = new FileOutputStream(file, true);
        if (!file.exists()) {
            file.createNewFile();
        }
        //阻塞等待客户端连接
        Socket socket = serverSocket.accept();
        //播放
        AudioFormat audioFormat = new AudioFormat(11000, 16, 1, true, false);
        SourceDataLine.Info info = new DataLine.Info(SourceDataLine.class, audioFormat, 1024);
        SourceDataLine sourceDataLine = (SourceDataLine) AudioSystem.getLine(info);
        sourceDataLine.open(audioFormat);
        sourceDataLine.start();

        socket.getInetAddress();
        System.out.println("客户端:" + InetAddress.getLocalHost() + "已连接到服务器");
        // 装饰流BufferedReader封装输入流（接收客户端的流）
        BufferedInputStream bis = new BufferedInputStream(
                socket.getInputStream());
        byte[] buffer = new byte[2];

        while (bis.read(buffer) != -1) {
            //保存至文件
            fileOutputStream.write(buffer);
            fileOutputStream.flush();
            //实时播放PCM
            sourceDataLine.write(buffer, 0, 2);


        }


    }

}

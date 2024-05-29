package pro.dengyi;

import javax.sound.sampled.SourceDataLine;

/**
 * @author dengyi (email:dengyi@dengyi.pro)
 * @date 2021-10-08
 */
public class PlayThread implements Runnable {
    private SourceDataLine sourceDataLine;
    private byte[] buffer;


    public PlayThread(SourceDataLine sourceDataLine, byte[] buffer) {
        this.sourceDataLine = sourceDataLine;
        this.buffer = buffer;
    }

    @Override
    public void run() {
        sourceDataLine.write(buffer, 0, 2);
    }
}

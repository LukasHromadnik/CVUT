package mas.agents;

import java.util.concurrent.LinkedBlockingQueue;

public class SequencedOutput {

    private static final LinkedBlockingQueue<String> messages = new LinkedBlockingQueue<>();

    public static void println(String str) {
        messages.offer(str);
    }

    static {
        new Thread() {
            public void run() {
                while(true) {
                    try {
                        System.out.println(messages.take());
                    } catch(InterruptedException ie) {}
                }
            }
        }.start();
    }

}

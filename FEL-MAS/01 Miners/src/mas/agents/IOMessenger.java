package mas.agents;

import java.io.*;

public class IOMessenger extends AbstractMessenger {

    private final PrintStream out;
    private final BufferedReader in;

    private final Thread receivingThread;

    public IOMessenger(int agentId, InputStream is, OutputStream os) throws IOException {
        super(agentId);

        out = new PrintStream(os);
        in = new BufferedReader(new InputStreamReader(is));

        receivingThread = new Thread(new Runnable() {
            @Override
            public void run() {
                while(true) {
                    try {
                        receive((Message) Serde.deserialize(in.readLine()));
                    } catch(Exception e) {
                        e.printStackTrace();
                        throw new RuntimeException();
                    }
                }
            }
        });
        receivingThread.start();
    }

    @Override
    protected synchronized void send(Message m) throws IOException {
        out.println(Serde.serialize(m));
        out.flush();
    }

    public static MessengerFactory factory = new MessengerFactory() {
        @Override
        public AbstractMessenger create(int agentId, InputStream is, OutputStream os) throws IOException {
            return new IOMessenger(agentId, is, os);
        }
    };

}

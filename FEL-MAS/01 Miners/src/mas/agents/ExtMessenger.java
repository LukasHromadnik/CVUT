package mas.agents;

import mas.agents.task.mining.DropMessage;
import mas.agents.task.mining.GrabMessage;
import mas.agents.task.mining.MoveMessage;
import mas.agents.task.mining.StatusRequestMessage;

import java.io.*;

public class ExtMessenger extends AbstractMessenger {

    private final PrintStream out;
    private final BufferedReader in;

    private final Thread receivingThread;

    public ExtMessenger(int agentId, InputStream is, OutputStream os) throws IOException {
        super(agentId);

        out = new PrintStream(os);
        in = new BufferedReader(new InputStreamReader(is));

        receivingThread = new Thread(new Runnable() {
            @Override
            public void run() {
                while(true) {
                    try {
                        String msg = in.readLine();
                        int delim = msg.indexOf(':');
                        String[] header = msg.substring(0, delim).split("\\s+");
                        String content = msg.substring(delim+1).trim();

                        Message obj = null;

                        if(content.equals("!left")) {
                            obj = new MoveMessage(-1.0, 0.0);
                        } else if(content.equals("!right")) {
                            obj = new MoveMessage(1.0, 0.0);
                        } else if(content.equals("!up")) {
                            obj = new MoveMessage(0.0, -1.0);
                        } else if(content.equals("!down")) {
                            obj = new MoveMessage(0.0, 1.0);
                        } else if(content.equals("!grab")) {
                            obj = new GrabMessage();
                        } else if(content.equals("!drop")) {
                            obj = new DropMessage();
                        } else if(content.equals("!sense")) {
                            obj = new StatusRequestMessage();
                        } else if(content.equals("!ready")) {
                            obj = new ReadyMessage();
                        } else {
                            obj = new UnknownMessage(content);
                        }

                        obj.setRecipient(Integer.parseInt(header[0]));
                        obj.setSender(Integer.parseInt(header[1]));
                        obj.setId(Long.parseLong(header[2]));

                        if(header[3].equals("Q")) obj.setQuery(true);
                        else if(header[3].startsWith("R")) obj.setReplyToken(Long.parseLong(header[3].substring(1)));

                        receive(obj);
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
        out.println(m.serialize());
        out.flush();
    }

    public static MessengerFactory factory = new MessengerFactory() {
        @Override
        public AbstractMessenger create(int agentId, InputStream is, OutputStream os) throws IOException {
            return new IOMessenger(agentId, is, os);
        }
    };

}

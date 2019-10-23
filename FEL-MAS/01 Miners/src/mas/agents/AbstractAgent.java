package mas.agents;

import mas.agents.task.mining.*;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;

public abstract class AbstractAgent extends Thread {

    public static final boolean ASYNC = false;

    public final AbstractMessenger messenger;
    private final int id;
    private SimulationApi api;

    public AbstractAgent(int id, InputStream is, OutputStream os, SimulationApi api) throws IOException, InterruptedException {
        messenger = new IOMessenger(id, is, os);
        this.id = id;
        this.api = api;
    }

    public void run() {
        try {
            messenger.send(0, new ReadyMessage());
            while (!(messenger.getIncomingMessage() instanceof StartMessage)) ;

            act();

            synchronized (this) {
                wait();
            }
        } catch(Exception e) {
            // Dispatch and log exception
            e.printStackTrace();
            try {
                synchronized (this) {
                    wait();
                }
            } catch(InterruptedException ie) {}
        }
    }

    public void sendMessage(int recipient, Message m) throws IOException {
        messenger.send(recipient, m);
    }

    public Message readMessage() throws InterruptedException {
        return messenger.getIncomingMessage();
    }

    public boolean messageAvailable() {
        return messenger.messageAvailable();
    }

    public StatusMessage left() throws IOException {
        return move(-1.0, 0.0);
    }
    public StatusMessage right() throws IOException {
        return move(1.0, 0.0);
    }
    public StatusMessage up() throws IOException {
        return move(0.0, -1.0);
    }
    public StatusMessage down() throws IOException {
        return move(0.0, 1.0);
    }
    public StatusMessage pick() throws IOException {
        return command(new GrabMessage());
    }
    public StatusMessage drop() throws IOException {
        return command(new DropMessage());
    }
    public StatusMessage sense() throws IOException {
        return command(new StatusRequestMessage());
    }

    private StatusMessage move(double dx, double dy) throws IOException {
        return command(new MoveMessage(dx, dy));
    }
    private StatusMessage command(Message msg) throws IOException {
        if(ASYNC) {
            messenger.send(0, msg);
            return null;
        } else return (StatusMessage)messenger.query(0, msg);
    }

    public void log(Object obj) throws Exception {
        api.println(String.format("Agent %d: %s", id, obj.toString()));
    }

    public int getAgentId() {
        return id;
    }

    public abstract void act() throws Exception;

}

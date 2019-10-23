package mas.agents;

import java.io.IOException;
import java.util.Map;
import java.util.concurrent.BlockingQueue;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.LinkedBlockingQueue;

public abstract class AbstractMessenger {

    protected int agentId;
    private final Map<Long,ReplyMessage> replies = new ConcurrentHashMap<>();
    private final BlockingQueue<Message> inbox = new LinkedBlockingQueue<>();

    public AbstractMessenger(int agentId) {
        this.agentId = agentId;
    }

    public Message query(int agent, Message m) throws IOException {
        m.setSender(agentId);
        m.setRecipient(agent);
        m.setQuery(true);

        ReplyMessage r = new ReplyMessage();
        replies.put(m.getId(), r);

        send(m);

        while(!replies.get(m.getId()).hasPayload()) {
            try {
                synchronized (r) {
                    r.wait(10);
                }
            } catch(InterruptedException ie) {
                ie.printStackTrace();
            }
        }

        return replies.remove(m.getId()).getPayload();
    }

    public void send(int agent, Message m) throws IOException {
        m.setSender(agentId);
        m.setRecipient(agent);
        m.setQuery(false);

        send(m);
    }

    protected void receive(Message m) throws IOException {
        m.setMessenger(this);

        if(m.getReplyToken() != null) {
            ReplyMessage r = replies.get(m.getReplyToken());
            r.setPayload(m);
            synchronized (r) {
                r.notifyAll();
            }
        } else {
            if(!inbox.offer(m)) {
                throw new RuntimeException("Message has not been added to the inbox!");
            }
        }
    }

    public Message getIncomingMessage() throws InterruptedException {
        return inbox.take();
    }

    public boolean messageAvailable() {
        return !inbox.isEmpty();
    }

    protected abstract void send(Message m) throws IOException;

}

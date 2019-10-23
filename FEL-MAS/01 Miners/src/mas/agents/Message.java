package mas.agents;

import java.io.IOException;
import java.io.Serializable;
import java.util.concurrent.atomic.AtomicLong;

public abstract class Message implements Serializable {

    private static final ThreadLocal<AtomicLong> messageCounter = new ThreadLocal<AtomicLong>() {
        @Override
        protected AtomicLong initialValue() {
            return new AtomicLong(0L);
        }
    };

    public Message() {
        id = messageCounter.get().getAndIncrement();
    }

    private long id;
    private int sender;
    private int recipient;
    private boolean query;

    private Long replyToken = null;

    private transient AbstractMessenger messenger;

    public long getId() {
        return id;
    }

    public final int getSender() {
        return sender;
    }

    protected final void setSender(int sender) {
        this.sender = sender;
    }

    public final int getRecipient() {
        return recipient;
    }

    protected final void setRecipient(int recipient) {
        this.recipient = recipient;
    }

    public final boolean isQuery() {
        return query;
    }

    protected final void setQuery(boolean query) {
        this.query = query;
    }

    protected Long getReplyToken() {
        return replyToken;
    }

    protected void setMessenger(AbstractMessenger messenger) {
        this.messenger = messenger;
    }

    protected void setId(long id) {
        this.id = id;
    }

    protected void setReplyToken(long replyToken) {
        this.replyToken = replyToken;
    }

    public void replyWith(Message m) throws IOException {
        if(isQuery()) m.replyToken = this.id;
        messenger.send(getSender(), m);
    }

    public String stringify() {
        return getClass().getSimpleName();
    }

    @Override
    public String toString() {
        return String.format("%s[%d->%d]%-10d %s", isQuery() ? "Q" : " ", getSender(), getRecipient(), getId(), stringify());
    }

    public String serialize() {
        throw new UnsupportedOperationException();
    }
}

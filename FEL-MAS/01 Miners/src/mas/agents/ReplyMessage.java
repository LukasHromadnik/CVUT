package mas.agents;

public class ReplyMessage {

    private Message payload;

    public ReplyMessage() {
        payload = null;
    }

    public void setPayload(Message payload) {
        this.payload = payload;
    }

    public boolean hasPayload() {
        return getPayload() != null;
    }

    public Message getPayload() {
        return payload;
    }
}

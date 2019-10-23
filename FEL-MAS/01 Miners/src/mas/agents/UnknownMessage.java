package mas.agents;

public class UnknownMessage extends Message {
    protected String content;
    protected boolean customSerializable = true;

    public UnknownMessage(String content) {
        this.content = content;
    }

    @Override
    public String serialize() {
        return String.format("%d %d %d %s : %s", getRecipient(), getSender(), getId(),
                isQuery() ? "Q" : (getReplyToken() != null ? "R"+getReplyToken() : "-"),
                content);
    }
}

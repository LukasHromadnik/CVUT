package mas.agents;

public class StartMessage extends Message {
    @Override
    public String serialize() {
        return String.format("%d %d %d %s : !start", getRecipient(), getSender(), getId(),
                isQuery() ? "Q" : (getReplyToken() != null ? "R"+getReplyToken() : "-"));
    }
}

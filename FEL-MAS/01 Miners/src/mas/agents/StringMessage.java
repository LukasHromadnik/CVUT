package mas.agents;

public class StringMessage extends Message {

    public String content;

    public StringMessage(String content) {
        this.content = content;
    }

    @Override
    public String stringify() {
        return content;
    }
}

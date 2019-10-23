package mas.agent.student;

import cz.agents.alite.communication.content.Content;

@SuppressWarnings("serial")
public class StringContent extends Content {
    String content;

    public StringContent(String content) {
        super(content);
        this.content = content;
    }

    @Override
    public String toString() {
        return "StringContent [content=" + content + "]";
    }

}

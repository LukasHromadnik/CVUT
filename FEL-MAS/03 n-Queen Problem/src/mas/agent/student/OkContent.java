package mas.agent.student;

public class OkContent extends StringContent {

    public OkContent(Integer content) {
        super(content.toString());
    }

    public Integer intContent() {
        return Integer.parseInt(content);
    }

    @Override
    public String toString() {
        return content;
    }
}

package mas.agent.student;

public class NogoodContent extends StringContent {

    private Integer[] agentView;

    public NogoodContent(Integer[] content) {
        super(content.toString());
        this.agentView = content;
    }

    @Override
    public String toString() {
        String result = "";
        for (int i = 0; i < agentView.length; i++) {
            result += "|";
            for (int j = 0; j < agentView.length; j++) {
                if (agentView[i] == j) {
                    result += "Q|";
                } else {
                    result += " |";
                }
            }
            result += "\n";
        }

        return result;
    }

    public Integer[] agentView() {
        return agentView;
    }
}

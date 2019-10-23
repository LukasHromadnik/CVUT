package mas.agent.student;

public class CheckViewContent extends StringContent {

    private Integer[] agentView;

    public CheckViewContent(Integer[] agentView) {
        super(agentView.toString());

        this.agentView = agentView;
    }

    public Integer[] agentView() {
        return agentView;
    }

}

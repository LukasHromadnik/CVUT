package mas.agents;

import java.io.InputStream;
import java.io.OutputStream;

public class AgentWrapper {

    public AbstractAgent agent;

    public InputStream is;
    public OutputStream os;

    public AgentWrapper(AbstractAgent agent, InputStream is, OutputStream os) {
        this.agent = agent;
        this.is = is;
        this.os = os;
    }
}

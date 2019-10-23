package mas.agents;

import java.io.IOException;

public interface TaskEnvironment {

    public void processMessage(Message m) throws IOException;

}

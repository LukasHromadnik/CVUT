package mas.agents;

import java.io.IOException;

public interface MessageHandler {

    void messageReceived(Message m) throws IOException;

}

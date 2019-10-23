package mas.agents;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;

public interface MessengerFactory {
    AbstractMessenger create(int agentId, InputStream is, OutputStream os) throws IOException;
}

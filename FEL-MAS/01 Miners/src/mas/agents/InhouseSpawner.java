package mas.agents;

import student.Agent;

import java.io.IOException;
import java.io.PipedInputStream;
import java.io.PipedOutputStream;

public class InhouseSpawner implements AgentSpawner {

    @Override
    public AgentWrapper spawn(int id) throws Exception {
        final PipedOutputStream os1 = new PipedOutputStream();
        final PipedInputStream is1 = new PipedInputStream(os1);

        final PipedOutputStream os2 = new PipedOutputStream();
        final PipedInputStream is2 = new PipedInputStream(os2);

        Agent ag = new Agent(id, is2, os1,
                new SimulationApi() {
                    @Override
                    public void println(String str) throws Exception {
                        SequencedOutput.println(str);
                    }
                });
        ag.start();

        return new AgentWrapper(ag, is1, os2);
    }

}

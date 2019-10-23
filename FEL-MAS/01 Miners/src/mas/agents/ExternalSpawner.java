package mas.agents;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;

public class ExternalSpawner implements AgentSpawner {

    private String command;

    public ExternalSpawner(String command) {
        this.command = command;
    }

    @Override
    public AgentWrapper spawn(int id) throws Exception {
        System.out.println("EXT SPAWNER");
        ProcessBuilder processBuilder =
                new ProcessBuilder((command + " " + id).split("\\s+"));
        final Process process = processBuilder.start();

        // Repeat error log:
        new Thread() {
            public void run() {
                try {
                    BufferedReader in = new BufferedReader(new InputStreamReader(process.getErrorStream()));
                    while(true) {
                        SequencedOutput.println(in.readLine());
                    }
                } catch(IOException ioe) {
                    ioe.printStackTrace();
                }
            }
        }.start();

        // https://stackoverflow.com/questions/269494/how-can-i-cause-a-child-process-to-exit-when-the-parent-does
        Thread closeChildThread = new Thread() {
            public void run() {
                process.destroy();
            }
        };
        Runtime.getRuntime().addShutdownHook(closeChildThread);

        return new AgentWrapper(null, process.getInputStream(), process.getOutputStream());
    }
}

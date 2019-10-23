package mas.agents;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;

public class ForkSpawner implements AgentSpawner {
    @Override
    public AgentWrapper spawn(int id) throws Exception {
        String separator = System.getProperty("file.separator");
        String classpath = System.getProperty("java.class.path");
        String path = System.getProperty("java.home")
                + separator + "bin" + separator + "java";
        ProcessBuilder processBuilder =
                new ProcessBuilder(path, "-cp",
                        classpath,
                        mas.agents.AgentCli.class.getName(),
                        Integer.toString(id));
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

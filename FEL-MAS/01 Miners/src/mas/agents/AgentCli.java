package mas.agents;

import student.Agent;

import java.io.*;

public class AgentCli {

    public static void main(String[] args) {
        try {
            System.setOut(new PrintStream(new OutputStream() {
                @Override
                public void write(int i) throws IOException {
                    System.err.write(i);
                }
            }));
            System.setIn(new InputStream() {
                @Override
                public int read() throws IOException {
                    return 0;
                }
            });

            int id = Integer.parseInt(args[0]);

            new Agent(id, new FileInputStream(FileDescriptor.in), new FileOutputStream(FileDescriptor.out),
                    new SimulationApi() {
                        @Override
                        public void println(String str) throws Exception {
                            System.err.println(str);
                        }
                    }).start();
        } catch(Exception e) {
            e.printStackTrace();
        }
    }

}

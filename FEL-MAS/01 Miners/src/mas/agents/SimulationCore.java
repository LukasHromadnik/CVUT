package mas.agents;

import mas.agents.task.mining.Environment;
import mas.agents.task.mining.MapConfig;

import java.io.File;
import java.io.IOException;

public class SimulationCore {

    public static void main(String[] args) throws Exception {
        String mapFile = args[0];
        AgentSpawner spawner = new InhouseSpawner();
        MessengerFactory msgFactory = IOMessenger.factory;

        if(args.length > 1) {
            if(args[1].equals("~")) spawner = new ForkSpawner();
            else {
                spawner = new ExternalSpawner(args[1]);
                msgFactory = ExtMessenger.factory;
            }
        }

        MapConfig config = MapConfig.load(new File(mapFile).getName(), SimulationCore.class.getResourceAsStream(mapFile));
        new SimulationCore(config, new Environment(config), spawner, msgFactory).run();
    }

    private TaskConfig config;
    private TaskEnvironment environment;
    private AgentSpawner spawner;
    private MessengerFactory msgFactory;

    private AbstractMessenger[] msgs;
    private AgentWrapper[] agents;
    private Thread[] receivingThreads;
    private boolean[] readyStates;

    private boolean running = false;

    public SimulationCore(TaskConfig config, TaskEnvironment environment, AgentSpawner spawner, MessengerFactory msgFactory) {
        this.config = config;
        this.environment = environment;
        this.spawner = spawner;
        this.msgFactory = msgFactory;
    }

    public synchronized void run() throws Exception {
        if(running) throw new IllegalStateException();

        msgs = new AbstractMessenger[config.getAgentsCount() + 1];
        agents = new AgentWrapper[config.getAgentsCount() + 1];
        receivingThreads = new Thread[config.getAgentsCount() + 1];
        readyStates = new boolean[config.getAgentsCount() + 1];

        for(int i = 1 ; i <= config.getAgentsCount() ; i++) {
            agents[i] = spawner.spawn(i);
            msgs[i] = msgFactory.create(0, agents[i].is, agents[i].os);

            final int threadId = i;
            receivingThreads[i] = new Thread() {
                @Override
                public void run() {
                    while(true) {
                        try {
                            Message m = msgs[threadId].getIncomingMessage();
                            m.setSender(threadId);
                            process(m);
                        } catch(Exception e) {
                            e.printStackTrace();
                            break;
                        }
                    }
                }
            };
            receivingThreads[i].start();
        }

        synchronized (this) {
            wait();
        }
    }

    private synchronized void process(Message m) throws IOException {
        if(m instanceof ReadyMessage) {
            readyStates[m.getSender()] = true;
            for(int i = 1 ; i <= config.getAgentsCount() ; i++) {
                if(!readyStates[i]) return;
            }

            running = true;
            for(int i = 1 ; i <= config.getAgentsCount() ; i++) {
                msgs[i].send(i, new StartMessage());
            }
        } else if(running == false) {
        } else if(m.getRecipient() != 0) {
            //System.out.println(m);
            msgs[m.getRecipient()].send(m);
        } else {
            environment.processMessage(m);
        }
    }
}

import java.util.ArrayList;
import java.util.HashMap;
import java.util.Timer;
import java.util.TimerTask;

/**
 * Created by Lukáš Hromadník on 22.11.16.
 */
public class CommandTimer extends Thread {

    private HashMap<Integer, ArrayList<Command>> commands;
    private Timer timer;
    private Integer time;

    public CommandTimer() {
        commands = new HashMap<Integer, ArrayList<Command>>();
        time = 0;
    }

    public void addCommandWithRepeat(Command command, Integer repeat) {
        ArrayList<Command> items = commands.get(repeat);
        if (items == null) {
            items = new ArrayList<Command>();
        }
        items.add(command);
        commands.put(repeat, items);
    }

    public void removeCommandWithRepeat(Command command, Integer repeat) {
        ArrayList<Command> items = commands.get(repeat);
        if (items == null) return;

        items.remove(command);

        if (items.size() == 0) {
            commands.remove(repeat);
        }
    }

    public void cancel() {
        if (timer == null) return;
        timer.cancel();
    }

    @Override
    public void run() {
        this.timer = new Timer();
        TimerTask task = new TimerTask() {
            @Override
            public void run() {
                for (Integer repeat : commands.keySet()) {
                    if (time % repeat != 0) continue;

                    for (Command command : commands.get(repeat)) {
                        command.execute();
                    }
                }
                time++;
            }
        };
        timer.scheduleAtFixedRate(task, 0, 1000);
    }
}

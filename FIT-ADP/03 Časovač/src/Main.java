import java.util.Timer;
import java.util.TimerTask;

public class Main {

    public static void main(String[] args) {
        CommandTimer commandTimer = new CommandTimer();

        NetworkRequestCommand request = new NetworkRequestCommand("https://jablickar.cz/feed/", null, null, null);
        commandTimer.addCommandWithRepeat(request, 3);

        PrintCommand printCommand = new PrintCommand("Hello World");
        commandTimer.addCommandWithRepeat(printCommand, 2);

        LogCommand logCommand = new LogCommand("log.txt", "ahoj");
        commandTimer.addCommandWithRepeat(logCommand, 1);

        LogCommand logCommand2 = new LogCommand("log.txt", "čau");
        commandTimer.addCommandWithRepeat(logCommand2, 1);

        commandTimer.start();

        TimerTask task = new TimerTask() {
            @Override
            public void run() {
                commandTimer.removeCommandWithRepeat(logCommand, 1);
            }
        };
        Timer timer = new Timer();
        timer.schedule(task, 2 * 1000 + 100);

        TimerTask task2 = new TimerTask() {
            @Override
            public void run() {
                commandTimer.removeCommandWithRepeat(request, 3);
            }
        };
        Timer timer2 = new Timer();
        timer.schedule(task2, 2 * 5000 + 100);

//        Timer timer2 = new Timer();
//        TimerTask task2 = new TimerTask() {
//            @Override
//            public void run() {
//                commandTimer.cancel();
//            }
//        };
//        timer2.schedule(task2, 4100);
    }
}

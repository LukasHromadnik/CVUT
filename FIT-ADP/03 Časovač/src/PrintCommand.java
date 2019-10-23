/**
 * Created by Lukáš Hromadník on 22.11.16.
 */
public class PrintCommand extends Thread implements Command {

    private String text;

    public PrintCommand(String text) {
        this.text = text;
    }

    @Override
    public void execute() {
        run();
    }

    @Override
    public void run() {
        System.out.println(text);
    }
}

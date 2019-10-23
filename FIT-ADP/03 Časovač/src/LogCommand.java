import java.io.FileWriter;
import java.io.PrintWriter;
import java.text.DateFormat;
import java.text.SimpleDateFormat;
import java.util.Date;

/**
 * Created by Lukáš Hromadník on 22.11.16.
 */
public class LogCommand extends Thread implements Command {

    private String filename;
    private String text;

    public LogCommand(String filename, String text) {
        this.filename = filename;
        this.text = text;
    }

    @Override
    public void run() {
        try {
            FileWriter writer = new FileWriter(filename, true);
            DateFormat dateFormat = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
            String message = dateFormat.format(new Date()) + " " +  text;
            writer.write(message);
            writer.write("\n");
            writer.close();
            System.out.println(message);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    @Override
    public void execute() {
        run();
    }
}

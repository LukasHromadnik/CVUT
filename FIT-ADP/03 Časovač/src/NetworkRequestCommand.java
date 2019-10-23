import java.util.Dictionary;
import java.io.*;
import java.net.*;
import java.util.HashMap;

/**
 * Created by Lukáš Hromadník on 22.11.16.
 */

public class NetworkRequestCommand extends Thread implements Command {

    private String path;
    private RequestMethod method;
    private HashMap<String, String> headers;
    private HashMap<String, String> data;

    public NetworkRequestCommand(String path, RequestMethod method, HashMap<String, String> headers, HashMap<String, String> data) {
        this.path = path;
        if (method == null) {
            method = RequestMethod.GET;
        }
        this.method = method;
        this.headers = headers;
        this.data = data;
    }

    @Override
    public void run() {
        try {
            URL url = new URL(path);
            HttpURLConnection connection = (HttpURLConnection) url.openConnection();

            connection.setRequestMethod(method.getMethod());
            connection.setRequestProperty("User-Agent", "Mozilla/5.0 (Windows; U; Windows NT 6.1; en-GB;     rv:1.9.2.13) Gecko/20101203 Firefox/3.6.13 (.NET CLR 3.5.30729)");

            if (headers != null) {
                for (String key : headers.keySet()) {
                    connection.addRequestProperty(key, headers.get(key));
                }
            }

            if (data != null) {
                connection.setDoOutput(true);
                String body = "";
                for (String key : data.keySet()) {
                    body += key + "=" + data.get(key) + "&";
                }
                if (body.length() > 2) {
                    body.substring(0, body.length() - 1);
                }
                DataOutputStream outputStream = new DataOutputStream(connection.getOutputStream());
                outputStream.writeBytes(body);
                outputStream.close();
                connection.setRequestProperty("Content-Length", Integer.toString(body.length()));
            }

            System.out.println(connection.getResponseCode());
            InputStream stream = connection.getErrorStream();
            if (stream == null) {
                stream = connection.getInputStream();
            }

            StringBuilder result = new StringBuilder();
            BufferedReader inputStream = new BufferedReader(new InputStreamReader(stream));
            String line;
            while ((line = inputStream.readLine()) != null) {
                result.append(line);
            }
            inputStream.close();
            System.out.println(result.toString());
        } catch(Exception e) {
            e.printStackTrace();
        }
    }

    @Override
    public void execute() {
        run();
    }
}

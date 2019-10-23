/**
 * Created by Lukáš Hromadník on 14.11.16.
 */
public class WriteThread extends Thread {

    private String name;
    private LargeContainerProxy proxy;
    private String key;
    private String value;

    public WriteThread(String name, LargeContainerProxy proxy, String key, String value) {
        this.name = name;
        this.proxy = proxy;
        this.key = key;
        this.value = value;
    }

    @Override
    public void run() {
        System.out.println("WriteThread [" + name +"] has started");
        proxy.set(key, value);
        System.out.println("WriteThread [" + name +"] has ended: " + proxy.toString());
    }
}

/**
 * Created by Lukáš Hromadník on 14.11.16.
 */

public class ReadThread extends Thread {

    private String name;
    private LargeContainerProxy proxy;
    private String key;

    public ReadThread(String name, LargeContainerProxy proxy, String key) {
        this.name = name;
        this.proxy = proxy;
        this.key = key;
    }

    @Override
    public void run() {
        System.out.println("ReadThread [" + name +"] has started");
        System.out.println("ReadThread [" + name +"] result: " + proxy.get(key));
        System.out.println("ReadThread [" + name +"] has ended: " + proxy.toString());
    }
}

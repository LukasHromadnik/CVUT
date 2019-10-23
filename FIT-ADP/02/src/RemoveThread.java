/**
 * Created by Lukáš Hromadník on 14.11.16.
 */

public class RemoveThread extends Thread {

    private String name;
    private LargeContainerProxy proxy;
    private String key;

    public RemoveThread(String name, LargeContainerProxy proxy, String key) {
        this.name = name;
        this.proxy = proxy;
        this.key = key;
    }

    @Override
    public void run() {
        System.out.println("RemoveThread [" + name +"] has started");
        proxy.remove(key);
        System.out.println("RemoveThread [" + name +"] has ended: " + proxy.toString());
    }
}

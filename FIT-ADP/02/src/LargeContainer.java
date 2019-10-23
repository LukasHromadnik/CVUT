/**
 * Created by Lukáš Hromadník on 14.11.16.
 */

import java.util.HashMap;

public class LargeContainer implements LargeContainerInterface {

    private static LargeContainer instance = null;

    private HashMap<String, String> data;

    private LargeContainer() {
        this.data = new HashMap<String, String>();
    }

    public static LargeContainer getInstance() {
        if (LargeContainer.instance == null) {
            LargeContainer.instance = new LargeContainer();
        }

        return LargeContainer.instance;
    }

    public LargeContainer clone() {
        LargeContainer newInstance = new LargeContainer();
        for (String key : data.keySet()) {
            newInstance.set(key, data.get(key));
        }

        return newInstance;
    }

    @Override
    public String get(String key) {
        return data.get(key);
    }

    @Override
    public void set(String key, String value) {
        data.put(key, value);
    }

    @Override
    public void remove(String key) {
        data.remove(key);
    }

    @Override
    public String toString() {
        return data.toString();
    }
}

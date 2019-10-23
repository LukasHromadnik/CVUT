/**
 * Created by Lukáš Hromadník on 14.11.16.
 */
public class CopyOnWriteStrategy implements ContainerStrategyInterface {
    @Override
    public String get(LargeContainer container, String key) {
        return container.get(key);
    }

    @Override
    public LargeContainer set(LargeContainer container, String key, String value) {
        LargeContainer newInstance = container.clone();
        newInstance.set(key, value);
        return newInstance;
    }

    @Override
    public LargeContainer remove(LargeContainer container, String key) {
        LargeContainer newInstance = container.clone();
        newInstance.remove(key);
        return newInstance;
    }
}

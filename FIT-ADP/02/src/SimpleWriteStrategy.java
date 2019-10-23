/**
 * Created by Lukáš Hromadník on 14.11.16.
 */
public class SimpleWriteStrategy implements ContainerStrategyInterface {

    @Override
    public String get(LargeContainer container, String key) {
        String value;
        synchronized (container) {
            value = container.get(key);
            try {
                Thread.sleep(3000);
            } catch (Exception e) {
                System.out.println(e.toString());
            }
        }
        return value;
    }

    @Override
    public LargeContainer set(LargeContainer container, String key, String value) {
        synchronized (container) {
            container.set(key, value);
        }
        return container;
    }

    @Override
    public LargeContainer remove(LargeContainer container, String key) {
        synchronized (container) {
            container.remove(key);
        }
        return container;
    }
}

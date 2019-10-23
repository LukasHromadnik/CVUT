/**
 * Created by Lukáš Hromadník on 14.11.16.
 */
public class LargeContainerProxy implements LargeContainerInterface {

    private LargeContainer container;
    private ContainerStrategyInterface strategy;

    public LargeContainerProxy(ContainerStrategyInterface strategy) {
        this.container = LargeContainer.getInstance();
        this.strategy = strategy;
    }

    @Override
    public String get(String key) {
        return strategy.get(container, key);
    }

    @Override
    public void set(String key, String value) {
        container = strategy.set(container, key, value);
    }

    @Override
    public void remove(String key) {
        container = strategy.remove(container, key);
    }

    @Override
    public String toString() {
        return container.toString();
    }
}

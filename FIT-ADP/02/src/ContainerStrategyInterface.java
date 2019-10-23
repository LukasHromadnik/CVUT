/**
 * Created by Lukáš Hromadník on 14.11.16.
 */
public interface ContainerStrategyInterface {

    public String get(LargeContainer container, String key);
    public LargeContainer set(LargeContainer container, String key, String value);
    public LargeContainer remove(LargeContainer container, String key);

}

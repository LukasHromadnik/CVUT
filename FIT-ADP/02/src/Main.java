public class Main {

    public static void main(String[] args) {
        LargeContainer container = LargeContainer.getInstance();
        container.set("Key1", "Value1");
        container.set("Key2", "Value2");
        container.set("Key3", "Value3");
        container.set("Key4", "Value4");
        container.set("Key5", "Value5");

        ReadThread t1 = new ReadThread("t1", new LargeContainerProxy(new SimpleWriteStrategy()), "Key2");
        WriteThread t2 = new WriteThread("t2", new LargeContainerProxy(new SimpleWriteStrategy()), "Key5", "New value");
        WriteThread t3 = new WriteThread("t3", new LargeContainerProxy(new CopyOnWriteStrategy()), "Key4", "Second new value");
        RemoveThread t4 = new RemoveThread("t4", new LargeContainerProxy(new SimpleWriteStrategy()), "Key1");
        ReadThread t5 = new ReadThread("t5", new LargeContainerProxy(new SimpleWriteStrategy()), "Key1");

        t1.start();
        t2.start();
        t3.start();

        try {
            t1.join();
            t2.join();
        } catch (Exception e) {
            System.out.println(e.toString());
        }

        t4.start();
        t5.start();
    }
}

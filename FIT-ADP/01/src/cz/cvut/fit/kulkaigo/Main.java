package cz.cvut.fit.kulkaigo;

import cz.cvut.fit.kulkaigo.Strategies.*;
import cz.cvut.fit.kulkaigo.Values.*;

public class Main {

    public static void main(String[] args) {

        AbstractStrategy strategy = new FifoStrategy();
        Cache cache = new Cache(5);

        // FIFO STRATEGY
        System.out.println("FIFO");
        cache.setStrategy(new FifoStrategy());
        cache.store(new IntKey(1), new IntValue(1000));
        cache.printCache();
        cache.store(new StringKey("1a"), new StringValue("1000a"));
        cache.printCache();
        cache.store(new IntKey(2), new IntValue(2000));
        cache.printCache();
        cache.store(new StringKey("2a"), new StringValue("2000a"));
        cache.printCache();
        cache.store(new IntKey(3), new IntValue(3000));
        cache.printCache();
        cache.store(new StringKey("3a"), new StringValue("3000a"));
        cache.printCache();
        cache.store(new IntKey(4), new IntValue(4000));
        cache.printCache();
        cache.store(new StringKey("4a"), new StringValue("4000a"));
        cache.printCache();

        cache.store(new IntKey(5), new IntValue(5000));
        cache.printCache();

        // LRU STRATEGY
        System.out.println("LRU");
        cache.setStrategy(new LruStrategy());
        cache.printCache();

        cache.get(new IntKey(3));
        cache.printCache();

        cache.store(new StringKey("5a"), new StringValue("5000a"));
        cache.printCache();
        cache.store(new IntKey(6), new IntValue(6000));
        cache.printCache();

        // LIFO STRATEGY
        System.out.println("LIFO");
        cache.setStrategy(new LifoStrategy());
        cache.printCache();

        cache.store(new StringKey("6a"), new StringValue("6000a"));
        cache.printCache();

        cache.store(new StringKey("7a"), new StringValue("7000a"));
        cache.printCache();

        cache.store(new StringKey("8a"), new StringValue("8000a"));
        cache.printCache();

        cache.store(new StringKey("9a"), new StringValue("9000a"));
        cache.printCache();

        // MRU STRATEGY
        System.out.println("MRU");
        cache.setStrategy(new MruStrategy());

        cache.printCache();

        cache.store(new StringKey("Key1"), new StringValue("Value1"));
        cache.printCache();

        cache.store(new StringKey("Key2"), new StringValue("Value2"));
        cache.printCache();

        cache.get(new StringKey("9a"));
        cache.printCache();

        cache.store(new StringKey("Key3"), new StringValue("Value3"));
        cache.printCache();

        cache.store(new StringKey("Key4"), new StringValue("Value4"));
        cache.printCache();

        cache.get(new IntKey(3));
        cache.printCache();

        cache.store(new StringKey("Key5"), new StringValue("Value5"));
        cache.printCache();

        // RANDOM REPLACEMENT STRATEGY
        System.out.println("Random replacement");
        cache.setStrategy(new RandomReplacementStrategy());

        cache.printCache();

        cache.store(new StringKey("Key6"), new StringValue("Value6"));
        cache.printCache();

        cache.store(new StringKey("Key7"), new StringValue("Value7"));
        cache.printCache();

        cache.store(new StringKey("Key8"), new StringValue("Value8"));
        cache.printCache();

        cache.store(new StringKey("Key9"), new StringValue("Value9"));
        cache.printCache();

        cache.store(new StringKey("Key10"), new StringValue("Value10"));
        cache.printCache();
    }
}

package cz.cvut.fit.kulkaigo;

import cz.cvut.fit.kulkaigo.Strategies.AbstractStrategy;
import cz.cvut.fit.kulkaigo.Values.AbstractKey;
import cz.cvut.fit.kulkaigo.Values.AbstractValue;

import java.util.Iterator;
import java.util.LinkedHashMap;
import java.util.Map;

/**
 * Created by Prozy on 25.10.2016.
 */
public class Cache {

    private AbstractStrategy strategy;
    private int cacheCapacity;
    private LinkedHashMap<AbstractKey, AbstractValue> cacheMap;

    public Cache(int capacity) {
        this.cacheMap = new LinkedHashMap<AbstractKey, AbstractValue>() {
            protected boolean removeEldestEntry(Map.Entry eldest) {
                return size() > capacity;
            }
        };
        this.setCacheCapacity(capacity);
    }

    public void setStrategy(AbstractStrategy strategy) {
        this.strategy = strategy;
    }

    public void store(AbstractKey key, AbstractValue value) {
        this.strategy.store(key, value, cacheMap);
    }

    public AbstractValue get(AbstractKey key) {
        return this.strategy.get(key, cacheMap);
    }

    public void printCache() {
        Iterator<Map.Entry<AbstractKey, AbstractValue>> itr = this.cacheMap.entrySet().iterator();
        while(itr.hasNext()){
            Map.Entry<AbstractKey, AbstractValue> entry = itr.next();
            System.out.print("(K: " + entry.getKey() + ", V: " + entry.getValue().toString() + ")");
        }
        System.out.print("\n");
    }

    public int getCacheCapacity() {
        return cacheCapacity;
    }

    public void setCacheCapacity(int cacheCapacity) {
        this.cacheCapacity = cacheCapacity;
    }
}

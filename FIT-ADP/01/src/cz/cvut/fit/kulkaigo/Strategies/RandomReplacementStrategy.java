package cz.cvut.fit.kulkaigo.Strategies;

import cz.cvut.fit.kulkaigo.Values.AbstractKey;
import cz.cvut.fit.kulkaigo.Values.AbstractValue;

import java.util.LinkedHashMap;
import java.util.Random;
import java.util.Set;

/**
 * Created by Luke on 01.11.16.
 */
public class RandomReplacementStrategy implements AbstractStrategy {

    public AbstractValue get(AbstractKey key, LinkedHashMap<AbstractKey, AbstractValue> cache) {
        return cache.get(key);
    }

    public void store(AbstractKey key, AbstractValue value, LinkedHashMap<AbstractKey, AbstractValue> cache) {
        Set<AbstractKey> keys = cache.keySet();

        int randomIndex = (new Random()).nextInt(keys.size());
        AbstractKey selectedKey = (AbstractKey) keys.toArray()[randomIndex];
        cache.remove(selectedKey);
        cache.put(key, value);
    }
}

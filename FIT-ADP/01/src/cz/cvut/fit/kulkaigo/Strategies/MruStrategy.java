package cz.cvut.fit.kulkaigo.Strategies;

import cz.cvut.fit.kulkaigo.Values.AbstractKey;
import cz.cvut.fit.kulkaigo.Values.AbstractValue;

import java.util.LinkedHashMap;
import java.util.Set;

/**
 * Created by Luke on 01.11.16.
 */
public class MruStrategy implements AbstractStrategy {
    @Override
    public AbstractValue get(AbstractKey key, LinkedHashMap<AbstractKey, AbstractValue> cache) {
        AbstractValue result = cache.get(key);

        LinkedHashMap<AbstractKey, AbstractValue> cacheBackup = (LinkedHashMap<AbstractKey, AbstractValue>) cache.clone();
        cacheBackup.remove(key);

        cache.clear();
        cache.put(key, result);
        cache.putAll(cacheBackup);

        return result;
    }
    @Override
    public void store(AbstractKey key, AbstractValue value, LinkedHashMap<AbstractKey, AbstractValue> cache) {
        Set<AbstractKey> keys = cache.keySet();
        if (keys.size() > 0) {
            AbstractKey lastKey = (AbstractKey) keys.toArray()[0];
            cache.remove(lastKey);
        }

        LinkedHashMap<AbstractKey, AbstractValue> cacheBackup = (LinkedHashMap<AbstractKey, AbstractValue>) cache.clone();

        cache.clear();
        cache.put(key, value);
        cache.putAll(cacheBackup);
    }
}

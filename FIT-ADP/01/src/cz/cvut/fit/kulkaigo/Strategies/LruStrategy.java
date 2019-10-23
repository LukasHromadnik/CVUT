package cz.cvut.fit.kulkaigo.Strategies;

import cz.cvut.fit.kulkaigo.Values.AbstractKey;
import cz.cvut.fit.kulkaigo.Values.AbstractValue;

import java.util.LinkedHashMap;
import java.util.Map;

/**
 * Created by Prozy on 25.10.2016.
 */
public class LruStrategy implements AbstractStrategy {
    @Override
    public AbstractValue get(AbstractKey key, LinkedHashMap<AbstractKey, AbstractValue> cache) {
        AbstractKey tmpKey = key;
        AbstractValue tmpVal = cache.get(key);

        cache.remove(key);
        cache.put(tmpKey, tmpVal);

        return tmpVal;
    }

    @Override
    public void store(AbstractKey key, AbstractValue value, LinkedHashMap<AbstractKey, AbstractValue> cache) {
        cache.put(key, value);
    }
}

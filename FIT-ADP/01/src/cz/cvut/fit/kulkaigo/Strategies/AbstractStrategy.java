package cz.cvut.fit.kulkaigo.Strategies;

import cz.cvut.fit.kulkaigo.Values.AbstractKey;
import cz.cvut.fit.kulkaigo.Values.AbstractValue;

import java.util.LinkedHashMap;

/**
 * Created by Prozy on 25.10.2016.
 */
public interface AbstractStrategy {

    public AbstractValue get(AbstractKey key, LinkedHashMap<AbstractKey, AbstractValue> cache);
    public void store(AbstractKey key, AbstractValue value, LinkedHashMap<AbstractKey, AbstractValue> cache);

}

package cz.cvut.fit.kulkaigo.Values;

/**
 * Created by Prozy on 25.10.2016.
 */
public class IntValue implements AbstractValue {

    private int value;

    public IntValue(int value) {
        this.setValue(value);
    }

    public int getValue() {
        return value;
    }

    public void setValue(int value) {
        this.value = value;
    }

    @Override
    public String toString() {
        return Integer.toString(this.getValue());
    }
}

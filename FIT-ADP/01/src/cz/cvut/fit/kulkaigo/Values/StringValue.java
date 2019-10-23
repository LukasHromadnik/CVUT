package cz.cvut.fit.kulkaigo.Values;

/**
 * Created by Prozy on 25.10.2016.
 */
public class StringValue implements AbstractValue {

    private String value;

    public StringValue(String value) {
        this.setValue(value);
    }

    public String getValue() {
        return value;
    }

    public void setValue(String value) {
        this.value = value;
    }

    @Override
    public String toString() {
        return this.getValue();
    }
}

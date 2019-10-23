package cz.cvut.fit.kulkaigo.Values;

/**
 * Created by Prozy on 25.10.2016.
 */
public class StringKey implements AbstractKey {

    private String key;

    public StringKey(String key) {
        this.setKey(key);
    }

    public String getKey() {
        return key;
    }

    public void setKey(String key) {
        this.key = key;
    }

    @Override
    public String toString() {
        return this.getKey();
    }

    @Override
    public int hashCode() {
        return this.toString().hashCode();
    }

    @Override
    public boolean equals(Object obj) {
        if(obj != null && obj instanceof StringKey){
            StringKey key2 = (StringKey) obj;
            if(key2.getKey() == this.getKey()){
                return true;
            }
        }
        return false;
    }
}

package cz.cvut.fit.kulkaigo.Values;

/**
 * Created by Prozy on 25.10.2016.
 */
public class IntKey implements AbstractKey {

    private int key;

    public IntKey(int key) {
        this.setKey(key);
    }

    public int getKey() {
        return key;
    }

    public void setKey(int key) {
        this.key = key;
    }

    @Override
    public String toString() {
        return Integer.toString(this.getKey());
    }

    @Override
    public int hashCode() {
        return this.toString().hashCode();
    }

    @Override
    public boolean equals(Object obj) {
        if(obj != null && obj instanceof IntKey){
            IntKey key2 = (IntKey) obj;
            if(key2.getKey() == this.getKey()){
                return true;
            }
        }
        return false;
    }
}
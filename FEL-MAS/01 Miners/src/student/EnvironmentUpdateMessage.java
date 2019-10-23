package student;

public class EnvironmentUpdateMessage extends PositionMessage {

    public int type;
    public boolean isRemoved;

    public EnvironmentUpdateMessage(int type, int x, int y) {
        this(type, x, y, false);
    }

    public EnvironmentUpdateMessage(int type, int x, int y, boolean isRemoved) {
        super(x, y);
        this.type = type;
        this.isRemoved = isRemoved;
    }

}

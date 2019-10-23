package student;

import mas.agents.Message;

public class PositionMessage extends Message {

    public int x;
    public int y;

    public PositionMessage(int x, int y) {
        this.x = x;
        this.y = y;
    }

    public Position getPosition() {
        return new Position(x, y);
    }
}

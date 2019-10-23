package student;

public class Position {

    public int x;
    public int y;

    public Position(int x, int y) {
        this.x = x;
        this.y = y;
    }

    public boolean isNextTo(Position position) {
        int diffX = Math.abs(x - position.x);
        int diffY = Math.abs(y - position.y);

        return Math.abs(diffX + diffY) == 1;
    }

    public void debug() {
        System.out.println(String.format("[%d %d]", x, y));
    }

    public boolean isOn(Position position) {
        return position.x == x && position.y == y;
    }

    @Override
    public boolean equals(Object o) {
        if (o == this) return true;
        if (o instanceof Position) {
            Position that = (Position)o;
            return x == that.x && y == that.y;
        }
        return false;
    }

    @Override
    public String toString() {
        return String.format("(%d %d)", x, y);
    }

    public int distanceFrom(Position position) {
        int distanceX = Math.abs(x - position.x);
        int distanceY = Math.abs(y - position.y);

        return distanceX + distanceY + 1;
    }
}

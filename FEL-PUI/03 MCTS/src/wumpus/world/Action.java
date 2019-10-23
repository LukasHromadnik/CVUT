package wumpus.world;

import javax.vecmath.Point2i;
import javax.vecmath.Tuple2i;


public enum Action {
    NORTH(0,-1),
    SOUTH(0,+1),
    EAST(+1,0),
    WEST(-1,0);


    Tuple2i dir;

    private Action(int x, int y) {
        this.dir = new Point2i(x,y);
    }

    public Point2i apply(Point2i state) {
        Point2i newPos = new Point2i(state);
        newPos.add(dir);
        return newPos;
    }

    public Tuple2i getDirection() {
        return dir;
    }
}
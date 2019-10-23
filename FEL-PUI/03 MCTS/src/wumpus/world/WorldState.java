package wumpus.world;

import javax.vecmath.Point2i;
import java.awt.*;
import java.util.Arrays;

public class WorldState {

    private Point2i agent;
    private Point2i[] wumpuses;
    private CellContent[][] map;
    private int actionsLeft;

    public WorldState(Point2i agent, Point2i[] wumpuses, CellContent[][] map, int actionsLeft){
        this.agent = agent;
        this.wumpuses = wumpuses;
        this.map = map;
        this.actionsLeft = actionsLeft;
    }

    public WorldState(CellContent[][] map, Point2i[] wumpuses, Point2i agent, int actionsLeft) {
    }

    public int getX() {
        return agent.x;
    }

    public int getY() {
        return agent.y;
    }

    public CellContent[][] getMap() {
        return map;
    }

    public Point2i[] getWumpuses() {
        return Arrays.copyOf(wumpuses, wumpuses.length);
    }

    public Point2i getAgent() {
        return new Point2i(agent);
    }

    public static CellContent[][] deepCopy(CellContent[][] original) {
        if (original == null) {
            throw new RuntimeException();
        }

        final CellContent[][] result = new CellContent[original.length][];
        for (int i = 0; i < original.length; i++) {
            result[i] = Arrays.copyOf(original[i], original[i].length);
        }
        return result;
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;

        WorldState that = (WorldState) o;

        if (actionsLeft != that.actionsLeft) return false;
        if (agent != null ? !agent.equals(that.agent) : that.agent != null) return false;
        // Probably incorrect - comparing Object[] arrays with Arrays.equals
        if (!Arrays.equals(wumpuses, that.wumpuses)) return false;
        return Arrays.deepEquals(map, that.map);
    }

    @Override
    public int hashCode() {
        int result = agent != null ? agent.hashCode() : 0;
        result = 31 * result + Arrays.hashCode(wumpuses);
        result = 31 * result + Arrays.deepHashCode(map);
        result = 31 * result + actionsLeft;
        return result;
    }

    @Override
    public String toString() {
        return "WS{" + agent +"," + Arrays.toString(wumpuses) +", al=" + actionsLeft + ", map=...}";
    }

    public int getActionsLeft() {
        return actionsLeft;
    }
}

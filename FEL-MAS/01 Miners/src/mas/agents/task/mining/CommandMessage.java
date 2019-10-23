package mas.agents.task.mining;

import mas.agents.Message;

import java.awt.geom.Point2D;

public abstract class CommandMessage extends Message {

    Point2D origin = null;
    long elapsed = 0L;
    long duration = 0L;

    public void setOrigin(Point2D origin) {
        this.origin = origin;
    }

    public void setDuration(long duration) {
        this.duration = duration;
    }
}

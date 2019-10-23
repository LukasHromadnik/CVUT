package mas.agents.task.mining;

public class MoveMessage extends CommandMessage {

    double dx;
    double dy;

    public MoveMessage(double dx, double dy) {
        this.dx = dx;
        this.dy = dy;
    }
}

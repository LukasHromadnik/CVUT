package student;

import mas.agents.Message;

public class HelpResponseMessage extends PositionMessage {

    public boolean value;

    public HelpResponseMessage(boolean value, int x, int y) {
        super(x, y);
        this.value = value;
    }

    public static HelpResponseMessage cannot(int x, int y) {
        return new HelpResponseMessage(false, x, y);
    }

    public static HelpResponseMessage can(int x, int y) {
        return new HelpResponseMessage(true, x, y);
    }
}

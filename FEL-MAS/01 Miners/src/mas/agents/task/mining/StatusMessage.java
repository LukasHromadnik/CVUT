package mas.agents.task.mining;

import mas.agents.Message;

import java.io.Serializable;
import java.util.List;

public class StatusMessage extends Message {

    public static final int OBSTACLE = 1;
    public static final int DEPOT = 2;
    public static final int GOLD = 3;
    public static final int AGENT = 4;

    public List<SensorData> sensorInput;
    public int agentX;
    public int agentY;
    public int width;
    public int height;

    public StatusMessage(int agentX, int agentY, int width, int height, List<SensorData> sensorInput) {
        this.sensorInput = sensorInput;
        this.agentX = agentX;
        this.agentY = agentY;
        this.width = width;
        this.height = height;
    }

    @Override
    public String serialize() {
        StringBuilder content = new StringBuilder();
        content.append(String.format("!percepts %d %d %dx%d", agentX, agentY, width, height));
        for (SensorData sensor : sensorInput) {
            char type = '?';
            switch(sensor.type) {
                case OBSTACLE: type = 'O'; break;
                case DEPOT: type = 'D'; break;
                case GOLD: type = 'G'; break;
                case AGENT: type = 'A'; break;
            }

            content.append(String.format(" %c%d,%d", type, sensor.x, sensor.y));
        }

        return String.format("%d %d %d %s : %s", getRecipient(), getSender(), getId(),
                isQuery() ? "Q" : (getReplyToken() != null ? "R"+getReplyToken() : "-"),
                content.toString());
    }

    public static class SensorData implements Serializable {
        public final int x;
        public final int y;
        public final int type;

        public SensorData(int x, int y, int type) {
            this.x = x;
            this.y = y;
            this.type = type;
        }
    }

    public boolean isAtGold() {
        return isAt(GOLD);
    }
    public boolean isAtDepot() {
        return isAt(DEPOT);
    }
    private boolean isAt(int type) {
        for(SensorData sd : sensorInput) {
            if(sd.type == type && sd.x == agentX && sd.y == agentY) return true;
        }
        return false;
    }

}

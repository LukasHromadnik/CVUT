package student;

import mas.agents.task.mining.StatusMessage;

import java.util.ArrayList;
import java.util.Random;

public class GameEnvironment {

    public static final int UNKNOWN = -1;
    public static final int NOTHING = 0;
    public static final int RESERVED = 10;

    public enum GameObjectType {
        GOLD(StatusMessage.GOLD, "1"),
        OBSTACLE(StatusMessage.OBSTACLE, "O"),
        DEPOT(StatusMessage.DEPOT, "D"),
        UNKNOWN(GameEnvironment.UNKNOWN, "?"),
        NOTHING(GameEnvironment.NOTHING, "-"),
        RESERVED(GameEnvironment.RESERVED, "*");

        private int statusIdentifier;
        private String debugRepresentation;

        GameObjectType(int statusIdentifier, String debugRepresentation) {
            this.statusIdentifier = statusIdentifier;
            this.debugRepresentation = debugRepresentation;
        }
    }

    public class GameObject {
        public GameObjectType type;
        public int value;

        public GameObject(GameObjectType type, int value) {
            this.type = type;
            this.value = value;
        }

        public GameObject(GameObjectType type) {
            this(type, 1);
        }
    }

    public Position depot = null;

    private GameObject[][] environment;
    private int width;
    private int height;
    private boolean hasUndiscoveredPositions = true;
    private boolean hasGoldPositions = true;

    public GameEnvironment(StatusMessage status) {
        this.width = status.width;
        this.height = status.height;

        this.environment = new GameObject[this.width][this.height];
        for (int i = 0; i < this.width; i++) {
            for (int j = 0; j < this.height; j++) {
                environment[i][j] = new GameObject(GameObjectType.UNKNOWN);
            }
        }
    }

    public void updateWith(EnvironmentUpdateMessage message) {
        if (message.isRemoved) {
            environment[message.x][message.y] = new GameObject(GameObjectType.NOTHING);
            return;
        }

        switch (message.type) {
            case StatusMessage.DEPOT:
                depot = message.getPosition();
                environment[depot.x][depot.y] = new GameObject(GameObjectType.DEPOT);
                break;
            case StatusMessage.GOLD:
                if (environment[message.x][message.y].type == GameObjectType.RESERVED) return;
                environment[message.x][message.y] = new GameObject(GameObjectType.GOLD);
                break;
            case GameEnvironment.NOTHING:
                setDiscovered(message.getPosition());
                break;
            case GameEnvironment.RESERVED:
                environment[message.x][message.y] = new GameObject(GameObjectType.RESERVED);
                break;
            case StatusMessage.OBSTACLE:
                environment[message.x][message.y] = new GameObject(GameObjectType.OBSTACLE);
                break;
            default:
                break;
        }
    }

    public boolean shouldUpdateWith(EnvironmentUpdateMessage message) {
        GameObject object = environment[message.x][message.y];

        return object.type.statusIdentifier != message.type;
    }

    @Override
    public String toString() {
        String result = "";
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                result += environment[j][i].type.debugRepresentation;
            }
            result += "\n";
        }

        return result;
    }

    private boolean canBeDiscovered(Position position) {
        return position.x >= 0 && position.x < width && position.y >= 0 && position.y < height;
    }

    public boolean isDiscovered(Position position) {
        if ( ! canBeDiscovered(position)) return true;

        return environment[position.x][position.y].type.statusIdentifier > GameObjectType.UNKNOWN.statusIdentifier;
    }

    public void setDiscovered(Position position) {
        if (isDiscovered(position)) return;

        environment[position.x][position.y] = new GameObject(GameObjectType.NOTHING);
    }

    public boolean isReserved(Position position) {
        return environment[position.x][position.y].type == GameObjectType.RESERVED;
    }

    public GameObjectType typeAt(Position position) {
        return environment[position.x][position.y].type;
    }

    public Position getRandomUndiscoveredPosition() {
        if ( ! hasUndiscoveredPositions) return getRandomGoldPosition();

        ArrayList<Position> possiblePositions = new ArrayList<>();

        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                Position possiblePosition = new Position(j, i);
                if (typeAt(possiblePosition) != GameObjectType.UNKNOWN) continue;
                possiblePositions.add(possiblePosition);
            }
        }

        if (possiblePositions.size() == 0) {
            hasUndiscoveredPositions = false;
            return null;
        }

        Random random = new Random();
        int index = random.nextInt(possiblePositions.size());

        return possiblePositions.get(index);
    }

    private Position getRandomGoldPosition() {
        if ( ! hasGoldPositions) return null;

        ArrayList<Position> possiblePositions = new ArrayList<>();

        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                Position possiblePosition = new Position(j, i);
                if (typeAt(possiblePosition) != GameObjectType.GOLD && typeAt(possiblePosition) != GameObjectType.RESERVED) continue;
                possiblePositions.add(possiblePosition);
            }
        }

        if (possiblePositions.size() == 0) {
            hasGoldPositions = false;
            return null;
        }

        Random random = new Random();
        int index = random.nextInt(possiblePositions.size());

        return possiblePositions.get(index);
    }

}

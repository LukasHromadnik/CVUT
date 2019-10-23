import javafx.scene.layout.Priority;

import java.util.Queue;
import java.util.Stack;

public class RobotEmilAgent {
	/**
	 * This method is called after when a simulation engine request next action.
	 * You are given a position of the robot and the map of the environment.
	 *
	 * The top-left corner has coordinates (0,0). You can check whether
	 * there is an obstacle by querying map[x][y] == CellContent.Obstacle.
	 *
	 * There is one gold on the map. You can query whether a position contains gold by
	 * querying map[x][y] == CellContent.Gold.
	 *
	 * @param x the x-coordinate of the current position of robot
	 * @param y the y-coordinate of the current position of robot
	 * @param map the map of the environment
	 * @return action to perform in the next step
	 */

	public Integer goldX = null;
	public Integer goldY = null;

	public Stack<SearchNode> plan = new Stack<>();

	public Action nextStep(int x, int y, CellContent[][] map) {

	    findGoldIfNeeded(map);

	    createPlanIfNeeded(x, y, map);

	    SearchNode node = plan.pop();

	    if (node.position.x - 1 == x) {
	        return Action.WEST;
        } else if (node.position.x + 1 == x) {
	        return Action.EAST;
        } else if (node.position.y - 1 == y) {
	        return Action.NORTH;
        } else {
	        return Action.SOUTH;
        }
	}

	private void createPlanIfNeeded(int x, int y, CellContent[][] map) {
	    if (plan.size() > 0) {
            SearchNode topNode = plan.peek();

            if (topNode != null && topNode.position.x == x && topNode.position.y == y) return;
        }

        Integer[][] cells = new Integer[map.length][map[0].length];

	    Queue<SearchNode> queue = new Queue<SearchNode>();

	    SearchNode firstNode = new SearchNode();
        firstNode.position = new Position(x, y);
        firstNode.distance = 0;
        firstNode.estimate = distanceToTheGold(x, y);
        queue.add(firstNode);

        while ( ! queue.isEmpty()) {
            SearchNode node = queue.poll();

            Position[] positions = new Position[4];
            positions[0] = new Position(x, y - 1);
            positions[1] = new Position(x - 1, y);
            positions[2] = new Position(x + 1, y);
            positions[3] = new Position(x, y + 1);

            for (int i = 0; i < 4; i++) {
                Position position = positions[i];

                if (!position.isAdmissible(map)) continue;

                SearchNode child = new SearchNode();
                child.distance = node.distance + 1;
                child.parent = node;
                child.position = position;
                child.estimate = distanceToTheGold(position.x, position.y);

                System.out.println(child.estimate);

                if (child.estimate == 0) {
                    plan.clear();

                    SearchNode parent = child;
                    while (parent != null) {
                        plan.add(parent);
                        parent = parent.parent;
                    }
                    return;
                }

                queue.add(child);
            }
        }
    }

	private void findGoldIfNeeded(CellContent[][] map) {
	    if (goldX != null && goldY != null) return;

        int cols = map.length;
        int rows = map[0].length;

	    for (int i = 0; i < cols; i++) {
	        for (int j = 0; j < rows; j++) {
	            if (map[i][j] == CellContent.GOLD) {
	                goldX = i;
	                goldY = j;
	                System.out.println(i);
                    System.out.println(j);
	                return;
                }
            }
        }
    }

    private int distanceToTheGold(int x, int y) {
	    return Math.abs(x - goldX) + Math.abs(y - goldY);
    }
}

class Position {
    public Integer x;
    public Integer y;

    public Position(Integer x, Integer y) {
        this.x = x;
        this.y = y;
    }

    public boolean isAdmissible(CellContent[][] map) {
        if (x < 0 || y < 0) return false;

        if (map[x][y] == CellContent.OBSTACLE) return false;

        return true;
    }
}

class SearchNode implements Comparable<SearchNode> {
    public Position position;
    public Integer distance;
    public Integer estimate;
    public SearchNode parent = null;

    public Integer value() {
        return distance + estimate;
    }

    @Override
    public int compareTo(SearchNode o) {
        if (this.value() > o.value()) {
            return 1;
        } else if (this.value() < o.value()) {
            return -1;
        }

        return 0;
    }
}
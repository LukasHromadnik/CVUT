import java.util.Comparator;
import java.util.HashMap;
import java.util.HashSet;
import java.util.LinkedList;
import java.util.Map;
import java.util.PriorityQueue;
import java.util.Set;

import javax.vecmath.Point2i;



public class RobotEmilReferenceAgent extends RobotEmilAgent{

	LinkedList<Point2i> currentPath = null;
	public Action nextStep(int x, int y, CellContent[][] map) {
		// the size of the map can be obtained as
		int cols = map.length; // the number of columns in the x-dimension,
		int rows = map[0].length; // the number of rows in the y-dimesnion

		// the bottom-right cell is therefore accessible as map[col-1][rows-1]
		// find gold

		Point2i gold = null;
		for (int xx=0; xx < cols; xx++) {
			for (int yy=0; yy < rows; yy++) {
				if (map[xx][yy] == CellContent.GOLD) {
					gold = new Point2i(xx,yy);
				}
			}
		}

		if (currentPath == null || !currentPath.getFirst().equals(new Point2i(x,y)) ) {
			// replan
			System.out.println("Replanning...");
			currentPath = findPath(new Point2i(x,y), gold, map);
		}

		assert currentPath.getFirst().equals(new Point2i(x,y));
		currentPath.removeFirst();

		assert !currentPath.isEmpty();
		Point2i next = currentPath.getFirst();

		if (next.x > x) {
			return Action.EAST;
		} else if (next.x < x) {
			return Action.WEST;
		} else if (next.y > y) {
			return Action.SOUTH;
		} else if (next.y < y) {
			return Action.NORTH;
		}

		assert false;
		return null;

	}

	public LinkedList<Point2i> findPath(Point2i from, Point2i to, CellContent[][] map) {
		final Map<Point2i, Integer> fValues = new HashMap<Point2i, Integer>();
		PriorityQueue<Point2i> open = new PriorityQueue<>(100, new Comparator<Point2i>() {

			@Override
			public int compare(Point2i p1, Point2i p2) {
				return fValues.get(p1) - fValues.get(p2);
			}
		});

		Set<Point2i> closed = new HashSet<Point2i>();
		Map<Point2i, Point2i> parents = new HashMap<Point2i, Point2i>();


		open.add(from);
		fValues.put(from, 0);

		while (!open.isEmpty()) {
			Point2i current = open.poll();
			closed.add(current);

			if (current.equals(to)) {
				// we have goal
				LinkedList<Point2i> path = new LinkedList<>();

				do {
					path.addFirst(current);
					current = parents.get(current);
				} while (current != null);

				return path;
			}

			Point2i[] neighbors = new Point2i[] {
				new Point2i(current.x+1, current.y),
				new Point2i(current.x-1, current.y),
				new Point2i(current.x, current.y-1),
				new Point2i(current.x, current.y+1)};

			for(Point2i neighbor : neighbors) {
				if (neighbor.x >= 0 && neighbor.x < map.length &&
					neighbor.y >= 0 && neighbor.y < map[0].length &&
					map[neighbor.x][neighbor.y] != CellContent.OBSTACLE &&
					!closed.contains(neighbor)) {

					parents.put(neighbor, current);
					fValues.put(neighbor, fValues.get(current) + 1);
					open.add(neighbor);
				}
			}

		}

		throw new RuntimeException("No route found...");

	}

}

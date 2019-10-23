import javax.vecmath.Point2i;
import java.util.List;
import java.util.Random;


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
	public Action nextStep(int x, int y, CellContent[][] map, int iteration) throws PlanFailure{

		// the size of the map can be obtained as
		int cols = map.length; // the number of columns in the x-dimension,
		int rows = map[0].length; // the number of rows in the y-dimesnion

		// the bottom-right cell is therefore accessible as map[col-1][rows-1]

		//Throw this exception to signify failed plan (failed in the Robust FF sense)
		if (false){
			throw new PlanFailure("Unplanned for deviation from the plan!");
		}

		return Action.SOUTH; // return one of Action.EAST, Action.WEST, Action.NORTH, Action.SOUTH
	}

	public void resetAgent(){
		// Use this method to prepare the agent for the new simulation run (i.e. reset fields) You dont have to delete the plan you prepared previously.
	}
}

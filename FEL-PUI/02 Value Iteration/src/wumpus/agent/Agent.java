package wumpus.agent;

import com.vividsolutions.jts.geom.impl.PackedCoordinateSequence;
import wumpus.world.*;
import wumpus.world.WorldState;

import javax.vecmath.Point2i;
import java.util.Set;


public class Agent implements WorldModel.Agent {

	private Action[][] policy;

	/**
	 * This method is called when the simulation engine requests the next action.
	 * You are given a position of the robot and the map of the environment.
	 *
	 * The top-left corner has coordinates (0,0). 
	 * 
	 * You can check whether there is an obstacle on a particular cell of the map 
	 * by querying map[x][y] == wumpus.world.CellContent.OBSTACLE.
	 *
	 * There is one gold on the map. You can query whether a position contains gold by
	 * querying map[x][y] == wumpus.world.CellContent.GOLD.
	 * 
	 * Further, there are several pits on the map. You can query whether a position contains pit by
	 * querying map[x][y] == wumpus.world.CellContent.PIT.
	 *
	 * @return action to perform in the next step
	 */

	public Action nextStep(WorldState state) {
		//---- value iteration ----
		if (policy == null) {
			DebugVis.initVis();
			policy = computePolicy(state.getMap());
		}

		return policy[state.getAgent().x][state.getAgent().y];
	}

	/**
	 * Compute an optimal policy for the agent.
	 * @param map map of the environment
	 * @return an array that contains for each cell of the environment one action,
	 * i.e. one of: Action.NORTH, Action.SOUTH, Action.EAST, Action.WEST.
	 */
	private Action[][] computePolicy(CellContent[][] map) {
		// the size of the map can be obtained as
		int cols = map.length; // the number of columns in the x-dimension,
		int rows = map[0].length; // the number of rows in the y-dimension

		Action[][] policy = new Action[cols][rows];

		float[][] stateValues = new float[cols][rows];
		/* You can visualize a value for each state like this... (Press "s" in the visualization window) */
		DebugVis.setStateValues(stateValues);

		float[][][] stateActionValues = new float[cols][rows][Action.values().length];
		/* You can visualize a value for each action at each state like this... (Press "a" in the visualization window) */
		DebugVis.setStateActionValues(stateActionValues);

		for (int i = 0; i < cols; i++) {
		    for (int j = 0; j < rows; j++) {
		        stateValues[i][j] = 0;
            }
        }

        Action[] actions = { Action.NORTH, Action.EAST, Action.SOUTH, Action.WEST };
		int horizon = 200;
		for (int h = 0; h < horizon; h++) {
		    for (int i = 0; i < cols; i++) {
		        for (int j = 0; j < rows; j++) {
                    float maxStateValue = -1000;
                    Action maxAction = Action.NORTH;

                    for (Action action : actions) {
                        float actionValue = 0;
                        Set<Transition> transitions = WorldModel.getTransitions(new WorldState(new Point2i(i, j), new Point2i[0], map), action);
                        for (Transition transition : transitions) {
                            Point2i newPosition = transition.successorState.getAgent();
                            actionValue += transition.probability * (stateValues[newPosition.x][newPosition.y] + transition.reward);
                        }
                        if (actionValue > maxStateValue) {
                            maxStateValue = actionValue;
                            maxAction = action;
                        }
                    }

                    stateValues[i][j] = maxStateValue;
                    policy[i][j] = maxAction;
                }
            }
        }

		/* You can visualize a policy (optimal action at each state) like this... (Press "p" in the visualization window) */
		DebugVis.setPolicy(policy);

		return policy;
	}



}

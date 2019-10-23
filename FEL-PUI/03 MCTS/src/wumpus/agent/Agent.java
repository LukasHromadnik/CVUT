package wumpus.agent;

import wumpus.world.*;
import javax.vecmath.Point2i;
import java.util.Random;
import java.util.Set;


public class Agent implements WorldModel.Agent {

	/**
	 * This method is called when the simulation engine requests the next action.
	 * You are given a state of the world that consists of position of agent, positions of wumpuses, and the map of world.
	 * <p>
	 * The top-left corner has coordinates (0,0).
	 * <p>
	 * You can check the current position of agent through state.getAgent(), the positions of all
	 * wumpuses can be obtained via state.getWumpuses() and the map of world through state.getMap().
	 * <p>
	 * <p>
	 * You can check whether there is an obstacle on a particular cell of the map
	 * by querying state.getMap()[x][y] == CellContent.OBSTACLE.
	 * <p>
	 * There is one gold on the map. You can query whether a position contains gold by
	 * querying state.getMap()[x][y] == CellContent.GOLD.
	 * <p>
	 * Further, there are several pits on the map. You can query whether a position contains pit by
	 * querying state.getMap()[x][y] == CellContent.PIT.
	 *
	 * @return action to perform in the next step
	 */

	private Random rnd = new Random();
	private Action[][] policy;
	private CellContent[][] lastState;

	public Action nextStep(WorldState state) {
		// Value iteration
		if (policy == null || lastState == null || lastState != state.getMap()) {
            DebugVis.initVis();
            policy = computePolicy(state);
            lastState = state.getMap();
        }

		Action bestAction = Action.NORTH;
		double bestReward = -Double.MAX_VALUE;
		int horizon = 15;
		int numberOfAttempts = 8;
		for (Action action : WorldModel.getActions(state)) {
			double reward = simulateAction(action, horizon, numberOfAttempts, copyState(state));

//			System.out.println(action + " has reward " + reward);

			if (reward > bestReward) {
				bestReward = reward;
				bestAction = action;
			}
		}

//		System.out.println("=====\nChoose " + bestAction + " with reward " + bestReward + "\n");

//		bestAction = nextActionInState(state);

//        System.exit(0);

		return bestAction;
	}

	private WorldState copyState(WorldState state) {
		CellContent[][] map = WorldState.deepCopy(state.getMap());
		Point2i[] wumpuses = state.getWumpuses();
		Point2i agent = state.getAgent();
		int actionsLeft = state.getActionsLeft();
		return new WorldState(agent, wumpuses, map, actionsLeft);
	}

	private double simulateAction(Action action, int horizon, int numberOfAttempts, WorldState state) {
		double reward = 0;
		double discountFactor = 0.7;
		for (int i = 0; i < numberOfAttempts; i++) {
            WorldState newState = state;
            Action newAction = action;
			for (int h = 0; h <= horizon; h++) {
//			    System.out.println("Attempt (horizon): " + i + " (" + h + ")");
//			    System.out.println("Before: " + newState);
//			    System.out.println("Action: " + newAction);
				Outcome outcome = WorldModel.performAction(newState, newAction, this.rnd);
				reward += Math.pow(discountFactor, h) * outcome.reward;
				newState = outcome.state;
                newAction= nextActionInState(newState);
//                System.out.println("After: " + newState);
//                System.out.println("Next action: " + newAction);
//                System.out.println("Reward: " + outcome.reward);
//                System.out.println("-----");
			}
//			System.out.println();
		}
		return reward / numberOfAttempts;
	}

	private Action nextActionInState(WorldState state) {
		return policy[state.getAgent().x][state.getAgent().y];
	}

	private Action randomAction(WorldState state) {
		Random rand = new Random();
		Set<Action> actions = WorldModel.getActions(state);
		int actionIndex = rand.nextInt(actions.size());
		return actions.toArray(new Action[0])[actionIndex];
	}

	private Action[][] computePolicy(WorldState state) {
//		int length = state.getMap().length;
//		int height = state.getMap()[0].length;

//		Action[][] policy = new Action[length][height];

//		for (int i = 0; i < length; i++) {
//		    for (int j = 0; j < height; j++) {
//		        policy[i][j] = randomAction(state);
//            }
//        }

//        policy = computeValueIteration(state);

		return computeValueIteration(state);
	}

    private Action[][] computeValueIteration(WorldState state) {
        int cols = state.getMap().length; // the number of columns in the x-dimension,
        int rows = state.getMap()[0].length; // the number of rows in the y-dimension

        Action[][] policy = new Action[cols][rows];

        float[][] stateValues = new float[cols][rows];

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
                        Set<Transition> transitions = AgentWorldModel.getTransitions(new WorldState(new Point2i(i, j), new Point2i[0], state.getMap(), state.getActionsLeft()), action);
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
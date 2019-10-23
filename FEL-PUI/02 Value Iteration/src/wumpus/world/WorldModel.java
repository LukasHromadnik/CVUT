package wumpus.world;

import java.util.*;

import javax.vecmath.Point2i;


public class WorldModel {

    public interface Agent {
        public Action nextStep(WorldState state);
    }

    public static Outcome performAction(WorldState state, Action action, Random rnd) {

		double r = rnd.nextDouble();

        Transition[] transitions = getTransitions(state, action).toArray(new Transition[0]);

        Transition sampledTrans = null;
        double pTresh = 0;
        for (int i=0; i < transitions.length; i++) {
            if ( (r >= pTresh) && ( r < pTresh + transitions[i].probability)) {
                sampledTrans = transitions[i];
            }
            pTresh += transitions[i].probability;
        }

        if (sampledTrans == null) {
           System.out.println("No transition sampled");
        }
        assert Math.abs(pTresh - 1.0) < 0.001;

        return new Outcome(sampledTrans.successorState, sampledTrans.reward);
	}
	
	/**
	 * Returns a set of all transitions that may occur if 
	 * the given action is performed in the given state.
	 * 
	 * @param state the state in which is the action applied
	 * @param action the action applied
	 * @return the set of transition, where is transition is assigned a probability, next state and the reward received.
	 */

	public static Set<Transition> getTransitions(WorldState state, Action action) {
		Set<Transition> transitions = new HashSet<Transition>();
		
		if (state.getMap()[state.getX()][state.getY()] == CellContent.EMPTY) {
			
			switch (action) {
				case NORTH:
				{
					float prob = 0.8f;
					WorldState succState = moveAgent(state, Action.NORTH);
					float reward = reward(state, succState);
					transitions.add(new Transition(prob, succState, reward));
				}
				{
					float prob = 0.1f;
					WorldState succState = moveAgent(state, Action.WEST);
					float reward = reward(state, succState);
					transitions.add(new Transition(prob, succState, reward));
				}
				{
					float prob = 0.1f;
					WorldState succState = moveAgent(state, Action.EAST);
					float reward = reward(state, succState);
					transitions.add(new Transition(prob, succState, reward));
				}
				break;
				
				case SOUTH:
				{
					float prob = 0.8f;
					WorldState succState = moveAgent(state, Action.SOUTH);
					float reward = reward(state, succState);
					transitions.add(new Transition(prob, succState, reward));
				}
				{
					float prob = 0.1f;
					WorldState succState = moveAgent(state, Action.WEST);
					float reward = reward(state, succState);
					transitions.add(new Transition(prob, succState, reward));
				}
				{
					float prob = 0.1f;
					WorldState succState = moveAgent(state, Action.EAST);
					float reward = reward(state, succState);
					transitions.add(new Transition(prob, succState, reward));
				}
				break;
				
				case EAST:
				{
					float prob = 0.8f;
					WorldState succState = moveAgent(state, Action.EAST);
					float reward = reward(state, succState);
					transitions.add(new Transition(prob, succState, reward));
				}
				{
					float prob = 0.1f;
					WorldState succState = moveAgent(state, Action.NORTH);
					float reward = reward(state, succState);
					transitions.add(new Transition(prob, succState, reward));
				}
				{
					float prob = 0.1f;
					WorldState succState = moveAgent(state, Action.SOUTH);
					float reward = reward(state, succState);
					transitions.add(new Transition(prob, succState, reward));
				}
				break;
				
				case WEST:
				{
					float prob = 0.8f;
					WorldState succState = moveAgent(state, Action.WEST);
					float reward = reward(state, succState);
					transitions.add(new Transition(prob, succState, reward));
				}
				{
					float prob = 0.1f;
					WorldState succState = moveAgent(state, Action.NORTH);
					float reward = reward(state, succState);
					transitions.add(new Transition(prob, succState, reward));
				}
				{
					float prob = 0.1f;
					WorldState succState = moveAgent(state, Action.SOUTH);
					float reward = reward(state, succState);
					transitions.add(new Transition(prob, succState, reward));
				}	
				break;
			}
		} else {
			transitions.add(new Transition(1f, state, 0));
		}

        HashSet<Transition> newTransitions = new HashSet<Transition>();

		for (Transition t : transitions) {
            Point2i[] wumpuses = t.successorState.getWumpuses();

            Set<Transition> wumpusTransitions = new HashSet<Transition>();
            wumpusTransitions.add(t);

            for (int i=0; i < wumpuses.length; i++) {
                wumpusTransitions = addWumpusTransitions(wumpusTransitions, i);
            }

            newTransitions.addAll(wumpusTransitions);
        }

		return newTransitions;
	}
	
	private static Set<Transition> addWumpusTransitions(Set<Transition> oldTransitions, int wumpusId) {
        Set<Transition> newTransitions = new HashSet<Transition>();
        for (Transition t : oldTransitions) {
            Action[] actions = new Action[] {Action.EAST, Action.NORTH, Action.WEST, Action.SOUTH};
            for (Action action : actions) {

                float prob = t.probability * 0.25f;

                Point2i[] wumpuses = t.successorState.getWumpuses();
                wumpuses[wumpusId] = nextLocation(wumpuses[wumpusId], action, t.successorState.getMap());

                WorldState succState = new WorldState(t.successorState.getAgent(), wumpuses, t.successorState.getMap());
                float reward = t.reward;

                if (succState.getAgent().equals(wumpuses[wumpusId])) {
                    reward += (-100);
                }

                newTransitions.add(new Transition(prob, succState, reward));
            }
        }
        return newTransitions;
    }

    public static Set<Action> getActions(WorldState state) {
		if (state.getMap()[state.getAgent().x][state.getAgent().y] == CellContent.EMPTY) {
			Set<Action> actions = new HashSet<Action>();
			actions.add(Action.NORTH);
			actions.add(Action.SOUTH);
			actions.add(Action.EAST);
			actions.add(Action.WEST);
			return actions;
		} 
		
		return new HashSet<Action>() ;
	}

	public static boolean isTerminal(WorldState state) {
        CellContent[][] map = state.getMap();
        Point2i agent = state.getAgent();
	    boolean atGold = map[agent.x][agent.y] == CellContent.GOLD;
	    boolean inPit = map[agent.x][agent.y] == CellContent.PIT;
	    boolean metWumpus = Arrays.asList(state.getWumpuses()).contains(agent);

		return atGold || inPit || metWumpus;
	}
	
	private static float reward(WorldState fromState, WorldState toState) {

		CellContent[][] map = fromState.getMap();


		if (map[toState.getX()][toState.getY()] == CellContent.GOLD) {
			return 100-1;
		}

		if (map[toState.getX()][toState.getY()] == CellContent.PIT) {
			return -100-1;
		}

		if (map[toState.getX()][toState.getY()] == CellContent.EMPTY) {
			return -1;
		}

		if (map[toState.getX()][toState.getY()] == CellContent.OBSTACLE) {
			return -1;
		}

		throw new RuntimeException();
	}

    public static Point2i nextLocation(Point2i location, Action action, CellContent[][] map) {

	    Point2i desiredTarget = null;
        switch(action) {
            case NORTH:
                desiredTarget = new Point2i(location.x,location.y-1);
                break;

            case SOUTH:
                desiredTarget = new Point2i(location.x,location.y+1);
                break;

            case WEST:
                desiredTarget = new Point2i(location.x-1,location.y);
                break;

            case EAST:
                desiredTarget = new Point2i(location.x+1,location.y);
                break;
            default:
                new RuntimeException();
        }

        Point2i actualTarget = null;

        HashSet<CellContent> blockingCellContents = new HashSet<CellContent>();
        blockingCellContents.add(CellContent.OBSTACLE);

        if (desiredTarget.x < 0 || desiredTarget.x >= map.length || desiredTarget.y < 0 || desiredTarget.y >= map[0].length ||
                blockingCellContents.contains(map[desiredTarget.x][desiredTarget.y])) {
            actualTarget = new Point2i(location.x,location.y);
        } else {
            actualTarget = desiredTarget;
        }

        return actualTarget;
    }

    private static WorldState moveAgent(WorldState state, Action action) {
		Point2i actualTarget = nextLocation(new Point2i(state.getX(),state.getY()), action, state.getMap());
		return new WorldState(actualTarget, state.getWumpuses(), state.getMap());
	}

	public static Point2i[] generateWumpuses(int nWumpuses, CellContent map[][], Random rnd) {

		Point2i[] wumpuses = new Point2i[nWumpuses];

		for (int i=0; i<nWumpuses; i++) {

			int row = 0;
			int col = 0;

			do {
				col = rnd.nextInt(map.length);
				row = rnd.nextInt(map[col].length);
			} while (map[row][col] != CellContent.EMPTY);

			wumpuses[i] = new Point2i(col,row);
		}

		return wumpuses;
	}

    public static CellContent[][] generateMap(Random rnd, int cols, int rows,
                                        double obstRatio, double pitRatio, Point2i gold) {

        CellContent[][] map = new CellContent[cols][rows];

        for (int x=0; x < cols; x++) {
            for (int y=0; y < rows; y++){
                map[x][y] = CellContent.EMPTY;
            }
        }

        map[gold.x][gold.y] = CellContent.GOLD;


        for (int x=0; x < cols; x++) {
            for (int y=0; y < rows; y++){
                if (map[x][y] == CellContent.EMPTY) {
                    if (rnd.nextDouble() < obstRatio) {
                        map[x][y] = CellContent.OBSTACLE;
                    }
                }
            }
        }

        for (int x=0; x < cols; x++) {
            for (int y=0; y < rows; y++){
                if (map[x][y] == CellContent.EMPTY) {
                    if (rnd.nextDouble() < pitRatio) {
                        map[x][y] = CellContent.PIT;
                    }
                }
            }
        }


        return map;

    }

    public static Point2i getGoldPosition(WorldState state) {
        CellContent[][] map = state.getMap();
	    for (int x=0; x < map.length; x++) {
            for (int y=0; y < map[0].length; y++){
                if (map[x][y] == CellContent.GOLD) {
                    return new Point2i(x,y);
                }
            }
        }
        return null;
    }
}

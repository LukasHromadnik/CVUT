package wumpus.world;

import java.util.*;

import javax.vecmath.Point2i;

public class WorldModel {

    public interface Agent {
        Action nextStep(WorldState state);
    }

    public static Outcome performAction(WorldState state, Action action, Random rnd) {

        WorldState succState = null;
        float reward = 0;

        // sample outcome of an agent's move
        Set<Transition> agentTransitions = getTransitions(state, action);
        Transition agentTransition = sampleTransition(agentTransitions, rnd);
        succState = agentTransition.successorState;
        reward += agentTransition.reward;

        for (int wumpusId=0; wumpusId < state.getWumpuses().length; wumpusId++) {
            Set<Transition> wumpusTransitions = getWumpusTransitions(succState, wumpusId);
            Transition wumpusTransition = sampleTransition(wumpusTransitions, rnd);
            succState = wumpusTransition.successorState;
            reward += wumpusTransition.reward;
        }

        return new Outcome(succState, reward);
    }

    private static Transition sampleTransition(Set<Transition> transitionsSet, Random rnd) {
        Transition[] transitionsArr = transitionsSet.toArray(new Transition[0]);
        Transition sampledTrans = null;
        double r = rnd.nextDouble();
        double pTresh = 0;
        for (int i=0; i < transitionsArr.length; i++) {
            if ( (r >= pTresh) && ( r < pTresh + transitionsArr[i].probability)) {
                sampledTrans = transitionsArr[i];
            }
            pTresh += transitionsArr[i].probability;
        }

        if (sampledTrans == null) {
            System.out.println("No transition sampled");
        }
        assert Math.abs(pTresh - 1.0) < 0.001;

        return sampledTrans;
    }

    /**
     * Returns a set of all transitions that may occur if
     * the given action is performed in the given state.
     *
     * @param state the state in which is the action applied
     * @param action the action applied
     * @return the list of transition, where is transition is assigned a probability, next state and the reward received.
     */

    private static Set<Transition> getTransitions(WorldState state, Action action) {
        Set<Transition> transitions = new LinkedHashSet<Transition>();

        if (state.getMap()[state.getX()][state.getY()] == CellContent.EMPTY ||
                state.getMap()[state.getX()][state.getY()] == CellContent.GOLD ) {

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
            transitions.add(new Transition(1f, new WorldState(state.getAgent(), state.getWumpuses(), state.getMap(), 0), 0));
        }

        return transitions;
    }

    private static Set<Transition> getWumpusTransitions(WorldState state, int wumpusId) {
        Set<Transition> newTransitions = new LinkedHashSet<Transition>();
        Action[] actions = new Action[] {Action.EAST, Action.NORTH, Action.WEST, Action.SOUTH};
        for (Action action : actions) {
            float prob = 0.25f;

            Point2i[] wumpuses = state.getWumpuses();
            wumpuses[wumpusId] = nextLocation(wumpuses[wumpusId], action, state.getMap());

            WorldState succState = new WorldState(state.getAgent(), wumpuses, state.getMap(), state.getActionsLeft());
            float reward = 0;

            if (succState.getAgent().equals(wumpuses[wumpusId])) {
                reward += (-100);
            }

            newTransitions.add(new Transition(prob, succState, reward));
        }
        return newTransitions;
    }

    public static Set<Action> getActions(WorldState state) {
        if (state.getMap()[state.getAgent().x][state.getAgent().y] == CellContent.EMPTY || state.getMap()[state.getAgent().x][state.getAgent().y] == CellContent.GOLD) {
            Set<Action> actions = new LinkedHashSet<Action>();
            actions.add(Action.NORTH);
            actions.add(Action.SOUTH);
            actions.add(Action.EAST);
            actions.add(Action.WEST);
            return actions;
        }

        return new LinkedHashSet<Action>() ;
    }

    public static boolean isTerminal(WorldState state) {
        CellContent[][] map = state.getMap();
        Point2i agent = state.getAgent();
        boolean atGold = map[agent.x][agent.y] == CellContent.GOLD;
        boolean inPit = map[agent.x][agent.y] == CellContent.PIT;
        boolean metWumpus = Arrays.asList(state.getWumpuses()).contains(agent);

        return inPit || metWumpus || state.getActionsLeft() == 0;
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
            return -1.00f;
        }

        if (map[toState.getX()][toState.getY()] == CellContent.OBSTACLE) {
            return -1.00f;
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

        HashSet<CellContent> blockingCellContents = new LinkedHashSet<CellContent>();
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

        CellContent[][] map = state.getMap();
        if (map[actualTarget.x][actualTarget.y] == CellContent.GOLD) {
            map = WorldState.deepCopy(state.getMap());
            map[actualTarget.x][actualTarget.y] = CellContent.EMPTY;
        }

        return new WorldState(actualTarget, state.getWumpuses(), map, state.getActionsLeft()-1);
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
                                        double obstRatio, double pitRatio, double goldRatio) {

        CellContent[][] map = new CellContent[cols][rows];

        for (int x=0; x < cols; x++) {
            for (int y=0; y < rows; y++){
                map[x][y] = CellContent.EMPTY;
            }
        }

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

        for (int x=0; x < cols; x++) {
            for (int y=0; y < rows; y++){
                if (map[x][y] == CellContent.EMPTY) {
                    if (rnd.nextDouble() < goldRatio) {
                        map[x][y] = CellContent.GOLD;
                    }
                }
            }
        }


        return map;

    }
}

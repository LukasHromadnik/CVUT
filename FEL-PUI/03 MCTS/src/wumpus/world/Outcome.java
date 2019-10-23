package wumpus.world;

public class Outcome {
    public WorldState state;
    public double reward;
    public Outcome(WorldState state, double reward) {
        super();
        this.state = state;
        this.reward = reward;
    }
}
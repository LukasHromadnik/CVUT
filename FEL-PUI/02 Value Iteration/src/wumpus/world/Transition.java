package wumpus.world;


public class Transition {
	public final float probability;
    public final WorldState successorState;
    public final float reward;

	public Transition(float probability, WorldState successorState,
			float reward) {
		super();
		this.probability = probability;
		this.successorState = successorState;
		this.reward = reward;
	}
}
package wumpus;

import cz.agents.alite.vis.Vis;
import cz.agents.alite.vis.VisManager;
import cz.agents.alite.vis.VisManager.SceneParams;
import cz.agents.alite.vis.layer.common.ColorLayer;
import cz.agents.alite.vis.layer.terminal.TerminalLayer;
import cz.agents.alite.vis.layer.toggle.KeyToggleLayer;
import wumpus.agent.Agent;
import wumpus.agent.LabelLayer;
import wumpus.world.*;

import javax.vecmath.Point2d;
import javax.vecmath.Point2i;
import java.awt.*;
import java.util.Random;

public class Task2Creator {

    final static int COLS = 10;
    final static int ROWS = 10;
    final static double OBST_RATIO = 0.12;
    final static double PIT_RATIO = 0.15;
    final static double GOLD_RATIO = 0.10;
    final static int WUMPUSES = 0;
    final static int SEED = 210;
    final static int VISUALIZATION_DELAY = 10;

    final static Point2i INIT_POS = new Point2i(4,7);
    final static int STEPS = 50;

    WorldState state;

    WorldState lastState = null;
    Action lastAction = null;
    double lastUtility = 0;
    int lastStepCount = 0;

    private void initVisualization() {
        VisManager.setInitParam("Wumpus World", 1024, 768, 200, 500);
        VisManager.setSceneParam(new SceneParams() {

            @Override
            public Point2d getDefaultLookAt() {
                return new Point2d(COLS/2, ROWS/2);
            }

            @Override
            public double getDefaultZoomFactor() {
                return 20;
            }
        });

        VisManager.init();

        // Background
        VisManager.registerLayer(ColorLayer.create(Color.WHITE));

        // Draw world state
        VisManager.registerLayer(WorldStateLayer.create(new WorldStateLayer.WorldStateProvider() {
            @Override
            public WorldState getWorldState() {
                return state;
            }
        }));

        KeyToggleLayer toggleLayer = KeyToggleLayer.create("l");
        toggleLayer.addSubLayer(LabelLayer.create(COLS, ROWS));
        toggleLayer.setEnabled(false);
        VisManager.registerLayer(toggleLayer);

        // Draw last action
        VisManager.registerLayer(new TerminalLayer() {
            @Override
            public void paint(Graphics2D canvas) {
                super.paint(canvas);

                if (lastAction != null) {
                    canvas.setColor(Color.RED);

                    int x = Vis.transX((float) state.getX());
                    int y = Vis.transY((float) state.getY());

                    int l = Vis.transH(0.7);

                    if (lastAction == Action.NORTH)
                        canvas.drawLine(x, y, x, y - l);
                    else if (lastAction == Action.SOUTH)
                        canvas.drawLine(x, y, x, y + l);
                    else if (lastAction == Action.EAST)
                        canvas.drawLine(x, y, x + l, y);
                    else if (lastAction == Action.WEST)
                        canvas.drawLine(x, y, x - l, y);
                    else
                        System.out.println("Last action undefined...");
                }

                canvas.setFont(new Font("SansSerif", Font.BOLD, 16));
                canvas.setStroke(new BasicStroke(1));
                canvas.setColor(Color.BLACK);
                canvas.drawString(String.format("Total reward: %3.2f, Step: %d/%d", lastUtility, lastStepCount, STEPS),50,50);
            }
        });
    }

    private void create() {
        initVisualization();

        final int RUNS = 50;
        double sumUtility = 0;

        for (int i=0; i<RUNS; i++) {
            double utility = simulate(new Agent(), new Random(i), VISUALIZATION_DELAY);
            sumUtility += utility;
            System.out.println(">>> Simulation run finished. Utility: " + utility);
        }
        System.out.println(">>> " + RUNS + " simulation runs finished. Average utility: " + (sumUtility/RUNS) + "."  );
    }

    private double simulate(WorldModel.Agent agent, Random rnd, int visualizationDelay) {
        Random random = new Random(SEED);
        CellContent[][] map = WorldModel.generateMap(random, COLS, ROWS, OBST_RATIO, PIT_RATIO, GOLD_RATIO);
        Point2i[] wumpuses = WorldModel.generateWumpuses(WUMPUSES, map, random);
        state = new WorldState(INIT_POS, wumpuses, map, STEPS);

        double utility = 0;
        lastUtility = 0;
        lastStepCount = 0;

        while (state.getActionsLeft() > 0) {
            Action action = agent.nextStep(state);

            lastAction = action;
            lastState = state;

            try {Thread.sleep(visualizationDelay);} catch (InterruptedException e) {}

            Outcome outcome = WorldModel.performAction(state, action, rnd);
            utility += outcome.reward;
            state = outcome.state;
            lastUtility = utility;
            lastStepCount++;

            // visualize action execution
            lastAction = null;

            try { Thread.sleep(visualizationDelay); } catch (InterruptedException e) {}

            if (WorldModel.isTerminal(state)) {
                break;
            }
        }

        return utility;
    }

    public static void main(String[] args) {
        new Task2Creator().create();
    }
}

import java.awt.Color;
import java.util.Collection;
import java.util.Collections;
import java.util.LinkedList;
import java.util.Random;

import javax.vecmath.Point2d;

import tt.euclid2i.Point;
import tt.euclid2i.region.Circle;
import tt.euclid2i.region.Rectangle;
import tt.euclid2i.vis.RegionsLayer;
import tt.euclid2i.vis.RegionsLayer.RegionsProvider;
import cz.agents.alite.vis.VisManager;
import cz.agents.alite.vis.VisManager.SceneParams;
import cz.agents.alite.vis.layer.common.ColorLayer;

public class RobotEmilCreator {

	final static int CELLSIZE = 10;
	final static int COLS = 20;
	final static int ROWS = 20;
	final static double OBST_RATIO = 0.2;
	final static int STEPS = 200;
	final static int SIMULATION_STEP_DELAY = 250;

	CellContent[][] map; // First dimension is columns, second dimension is rows
	int x;
	int y;

    private void initVisualization() {
        VisManager.setInitParam("Robot Emil", 1024, 768, 200, 200);
        VisManager.setSceneParam(new SceneParams() {

            @Override
            public Point2d getDefaultLookAt() {
                return new Point2d(95, 95);
            }

            @Override
            public double getDefaultZoomFactor() {
                return 3;
            }
        });

        VisManager.init();

        // background
        VisManager.registerLayer(ColorLayer.create(Color.WHITE));

        // Draw robot
        VisManager.registerLayer(RegionsLayer.create(new RegionsProvider() {

			@Override
			public Collection<? extends tt.euclid2i.Region> getRegions() {
				Collection<Circle> regions = new LinkedList<Circle>();

				regions.add(new Circle(new Point(x*CELLSIZE, y*CELLSIZE),	CELLSIZE/2-2));

				return regions;
			}
		}, Color.BLACK, Color.BLUE));

        // Draw robot
        VisManager.registerLayer(RegionsLayer.create(new RegionsProvider() {

			@Override
			public Collection<? extends tt.euclid2i.Region> getRegions() {
				return Collections.singleton(new Rectangle(new Point(-CELLSIZE/2, -CELLSIZE/2), new Point(COLS * CELLSIZE - CELLSIZE/2, ROWS * CELLSIZE - CELLSIZE/2)));
			}
		}, Color.BLACK));

        // Draw obstacles
        VisManager.registerLayer(RegionsLayer.create(new RegionsProvider() {

			@Override
			public Collection<? extends tt.euclid2i.Region> getRegions() {
				Collection<Rectangle> regions = new LinkedList<Rectangle>();
		    	for (int x=0; x < COLS; x++) {
		    		for (int y=0; y < ROWS; y++){
		    			if (map[x][y] == CellContent.OBSTACLE) {
		    				regions.add(new Rectangle(
		    						new Point(x*CELLSIZE - CELLSIZE/2, y*CELLSIZE - CELLSIZE/2),
		    						new Point(x*CELLSIZE + CELLSIZE/2, y*CELLSIZE + CELLSIZE/2)
		    				));
		    			}

		    		}
		    	}
				return regions;
			}
		}, Color.BLACK, Color.GRAY));

        // Draw gold
        VisManager.registerLayer(RegionsLayer.create(new RegionsProvider() {

			@Override
			public Collection<? extends tt.euclid2i.Region> getRegions() {
				Collection<Circle> regions = new LinkedList<Circle>();
				for (int x=0; x < COLS; x++) {
		    		for (int y=0; y < ROWS; y++){
		    			if (map[x][y] == CellContent.GOLD) {
		    				regions.add(new Circle(new Point(x*CELLSIZE, y*CELLSIZE), CELLSIZE/2));
		    			}
		    		}
		    	}
				return regions;
			}
		}, Color.BLACK, Color.YELLOW));

        // Draw pit
        VisManager.registerLayer(RegionsLayer.create(new RegionsProvider() {

			@Override
			public Collection<? extends tt.euclid2i.Region> getRegions() {
				Collection<Circle> regions = new LinkedList<Circle>();
				for (int x=0; x < COLS; x++) {
		    		for (int y=0; y < ROWS; y++){
		    			if (map[x][y] == CellContent.PIT) {
		    				regions.add(new Circle(new Point(x*CELLSIZE, y*CELLSIZE), CELLSIZE/2));
		    			}

		    		}
		    	}
				return regions;
			}
		}, Color.BLACK, Color.BLACK));
    }

    private void create(RobotEmilAgent robot) {
    	initVisualization();

    	final int RUNS = 10;
    	int successCount = 0;

    	for (int i=0; i<RUNS; i++) {
    		generateMap(new Random(2));
	    	boolean success = simulate(robot, new Random(i), SIMULATION_STEP_DELAY);
	    	if (success) {
	    		successCount++;
	    	}
	    	System.out.println(">>> Simulation run finished. Found gold: " + success);
    	}
    	System.out.println(">>> " + RUNS + " simulation runs finished. " + (((double)successCount)/RUNS)*100 + "% finished successfuly."  );
    }

    private boolean simulate(RobotEmilAgent agent, Random rnd, int visualizationDelay) {
    	x = 0;
    	y = 0;

    	for (int i=0; i<STEPS; i++) {
    		Action action = agent.nextStep(x,y,map);
    		updatePosition(action, rnd);

    		if (map[x][y] == CellContent.GOLD) {
    			System.out.println("Your robot found gold.");
    			return true;
    		}

    		if (map[x][y] == CellContent.PIT) {
    			System.out.println("Your robot fell into a pit.");
    			return false;
    		}

    		try {
				Thread.sleep(visualizationDelay);
			} catch (InterruptedException e) {}
    	}
    	return false;
	}



	private void updatePosition(Action action, Random rnd) {
		int newX = x;
		int newY = y;
		if (action == Action.NORTH) {
			double r = rnd.nextDouble();
			if (r < 0.8)
				newY -= 1;
			else if (r < 0.9) {
				newX -= 1;
			} else {
				newX += 1;
			}
		}

		if (action == Action.SOUTH) {
			double r = rnd.nextDouble();
			if (r < 0.8)
				newY += 1;
			else if (r < 0.9) {
				newX -= 1;
			} else {
				newX += 1;
			}
		}

		if (action == Action.EAST) {
			double r = rnd.nextDouble();
			if (r < 0.8)
				newX += 1;
			else if (r < 0.9) {
				newY -= 1;
			} else {
				newY += 1;
			}
		}

		if (action == Action.WEST) {
			double r = rnd.nextDouble();
			if (r < 0.8)
				newX -= 1;
			else if (r < 0.9) {
				newX -= 1;
			} else {
				newX += 1;
			}
		}

		if (newX >= 0 && newX < COLS && newY >= 0 && newY < ROWS && map[newX][newY] != CellContent.OBSTACLE) {
			x = newX;
			y = newY;
		}

	}

	private void generateMap(Random rnd) {
    	x = 0;
    	y = 0;

    	map = new CellContent[COLS][ROWS];

    	for (int x=0; x < COLS; x++) {
    		for (int y=0; y < ROWS; y++){
    			map[x][y] = CellContent.EMPTY;
    		}
    	}

    	map[13][12] = CellContent.GOLD;
    	//map[9][5] = CellContent.PIT;

    	for (int x=0; x < COLS; x++) {
    		for (int y=0; y < ROWS; y++){
    			if (map[x][y] == CellContent.EMPTY) {
	    			if (rnd.nextDouble() < OBST_RATIO) {
	    				map[x][y] = CellContent.OBSTACLE;
	    			}
    			}
    		}
    	}

	}

	public static void main(String[] args) {
    	new RobotEmilCreator().create(new RobotEmilAgent());
    }
}

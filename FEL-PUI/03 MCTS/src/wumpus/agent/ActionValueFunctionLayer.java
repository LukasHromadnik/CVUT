package wumpus.agent;

import cz.agents.alite.vis.Vis;
import cz.agents.alite.vis.layer.VisLayer;
import cz.agents.alite.vis.layer.terminal.TerminalLayer;
import wumpus.world.Action;

import javax.vecmath.Tuple2i;
import java.awt.*;


public class ActionValueFunctionLayer extends TerminalLayer {

    static public interface ActionValueFunctionProvider {
       float[][][] getValueFunction();
    }

    ActionValueFunctionProvider provider;
	private float cellSize;

    ActionValueFunctionLayer(ActionValueFunctionProvider provider, int cellSize) {
        this.provider = provider;
        this.cellSize = cellSize;
    }

    @Override
    public void paint(Graphics2D canvas) {
    	canvas.setFont(new Font("SansSerif", Font.PLAIN, 9));
        canvas.setStroke(new BasicStroke(1));
        canvas.setColor(Color.GREEN.darker());

        float[][][] valueFunc = provider.getValueFunction();
        
        if (valueFunc != null) {
        
        int cols = valueFunc.length;
        int rows = valueFunc[0].length;
	        for (int i = 0; i < cols; i++) {
	        	for (int j = 0; j < rows; j++) {
	        		for (Action action : Action.values()) {
	        			Tuple2i dir = action.getDirection();
	        			String valueStr = String.format("%4.2f",valueFunc[i][j][action.ordinal()]);
		        		double strWidth = canvas.getFontMetrics().getStringBounds(valueStr, canvas).getWidth();
		                int x = Vis.transX((i * cellSize) + (dir.x * cellSize/3) ) - (int) strWidth/2;
		                int y = Vis.transY((j * cellSize) + (dir.y * cellSize/3) )  + canvas.getFontMetrics(canvas.getFont()).getAscent()/2;
		                canvas.drawString(valueStr, x, y);
	        		}
	        		
	        	}
				
			}
        }
    }

    public static VisLayer create(ActionValueFunctionProvider provider, int cellSize) {
        return new ActionValueFunctionLayer(provider,cellSize);
    }
}

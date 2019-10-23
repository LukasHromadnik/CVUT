package wumpus.agent;

import cz.agents.alite.vis.Vis;
import cz.agents.alite.vis.layer.VisLayer;
import cz.agents.alite.vis.layer.terminal.TerminalLayer;

import java.awt.*;


public class StateValueFunctionLayer extends TerminalLayer {

    static public interface ValueFunctionProvider {
       float[][] getValueFunction();
    }

    ValueFunctionProvider provider;
	private int cellSize;

    StateValueFunctionLayer(ValueFunctionProvider provider, int cellSize) {
        this.provider = provider;
        this.cellSize = cellSize;
    }

    @Override
    public void paint(Graphics2D canvas) {
    	canvas.setFont(new Font("SansSerif", Font.PLAIN, 9));
        canvas.setStroke(new BasicStroke(1));
        canvas.setColor(Color.GRAY);

        float[][] valueFunc = provider.getValueFunction();
        
        if (valueFunc != null) {
        
        int cols = valueFunc.length;
        int rows = valueFunc[0].length;
	        for (int i = 0; i < cols; i++) {
	        	for (int j = 0; j < rows; j++) {
	        		String valueStr = String.format("%4.2f",valueFunc[i][j]);
	        		double strWidth = canvas.getFontMetrics().getStringBounds(valueStr, canvas).getWidth();
	                int x = Vis.transX((i * cellSize)) - (int) strWidth/2;
	                int y = Vis.transY((j * cellSize))  + canvas.getFontMetrics(canvas.getFont()).getAscent()/2;
	                canvas.drawString(valueStr, x, y);
	        	}
				
			}
        }
    }

    public static VisLayer create(ValueFunctionProvider provider, int cellSize) {
        return new StateValueFunctionLayer(provider,cellSize);
    }
}

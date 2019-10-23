package wumpus.agent;

import cz.agents.alite.vis.Vis;
import cz.agents.alite.vis.layer.VisLayer;
import cz.agents.alite.vis.layer.terminal.TerminalLayer;
import java.awt.*;


public class LabelLayer extends TerminalLayer {

    int cols;
    int rows;

    public LabelLayer(int cols, int rows) {
        this.cols = cols;
        this.rows = rows;
    }

    @Override
    public void paint(Graphics2D canvas) {
    	canvas.setFont(new Font("SansSerif", Font.PLAIN, 9));
        canvas.setStroke(new BasicStroke(1));
        canvas.setColor(Color.GRAY);

        for (int i = 0; i < cols; i++) {
            for (int j = 0; j < rows; j++) {
                String valueStr = String.format("%d, %d", i, j);
                double strWidth = canvas.getFontMetrics().getStringBounds(valueStr, canvas).getWidth();
                int x = Vis.transX(i) - (int) strWidth/2;
                int y = Vis.transY(j) + canvas.getFontMetrics(canvas.getFont()).getAscent()/2;
                canvas.drawString(valueStr, x, y);
            }

        }
    }

    public static VisLayer create(int cols, int rows) {
        return new LabelLayer(cols, rows);
    }
}

package wumpus.world;

import cz.agents.alite.vis.Vis;
import cz.agents.alite.vis.layer.VisLayer;
import cz.agents.alite.vis.layer.terminal.TerminalLayer;

import javax.vecmath.Point2i;
import java.awt.*;


public class WorldStateLayer extends TerminalLayer {

    public interface WorldStateProvider {
       WorldState getWorldState();
    }

    WorldStateProvider provider;

    WorldStateLayer(WorldStateProvider provider) {
        this.provider = provider;
    }

    @Override
    public void paint(Graphics2D canvas) {
        canvas.setStroke(new BasicStroke(1));
        canvas.setColor(Color.BLUE);

        WorldState worldState = provider.getWorldState();

        if (worldState != null) {

            CellContent[][] map = worldState.getMap();
            int cols = map.length;
            int rows = map[0].length;
            int x = worldState.getX();
            int y = worldState.getY();

            for (int i = 0; i < cols; i++) {
                for (int j = 0; j < rows; j++) {

                    canvas.setStroke(new BasicStroke(1));
                    canvas.setColor(Color.GRAY);
                    canvas.drawRect(
                            Vis.transX(i - 0.5),
                            Vis.transY(j - 0.5),
                            Vis.transW(1.0), Vis.transH(1.0));

                    if (map[i][j] == CellContent.OBSTACLE) {

                        canvas.setColor(Color.DARK_GRAY);
                        canvas.fillRect(
                                Vis.transX(i - 0.5),
                                Vis.transY(j - 0.5),
                                Vis.transW(1.0), Vis.transH(1.0));
                    }

                    if (map[i][j] == CellContent.PIT) {

                        canvas.setColor(Color.BLACK);
                        canvas.fillOval(
                                Vis.transX(i - 0.5),
                                Vis.transY(j - 0.5),
                                Vis.transW(1.0), Vis.transH(1.0));
                    }

                    if (map[i][j] == CellContent.GOLD) {

                        canvas.setColor(Color.ORANGE);
                        canvas.fillOval(
                                Vis.transX(i - 0.5),
                                Vis.transY(j - 0.5),
                                Vis.transW(1.0), Vis.transH(1.0));
                    }
                }
            }

            // draw agent
            canvas.setColor(Color.BLUE);
            canvas.fillOval(Vis.transX(x - 0.4),
                    Vis.transY(y - 0.4),
                    Vis.transW(0.8), Vis.transH(0.8));

            // draw wumpuses
            for (Point2i wumpus : worldState.getWumpuses()) {
                canvas.setColor(Color.RED);
                canvas.fillOval(
                        Vis.transX(wumpus.x- 0.3),
                        Vis.transY(wumpus.y - 0.3),
                        Vis.transW(0.6), Vis.transH(0.6));
            }
        }
    }

    public static VisLayer create(WorldStateProvider provider) {
        return new WorldStateLayer(provider);
    }
}

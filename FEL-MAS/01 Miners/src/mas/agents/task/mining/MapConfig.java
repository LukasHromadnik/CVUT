package mas.agents.task.mining;

import mas.agents.TaskConfig;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.util.*;

public class MapConfig extends TaskConfig {

    private static final int OBSTACLE = -1;
    private static final int DEPOT = -2;

    private boolean guiEnabled = true;
    private int map[][];
    private List<Position> initialPositions;
    public int width;
    public int height;
    public int goldCount = 0;
    private int numAgents;

    private String name;

    private List<Position> depots = new ArrayList<>();
    private List<Position> obstacles = new ArrayList<>();
    private HashMap<Position,Integer> golds = new HashMap<>();

    public MapConfig(String name, int[][] map, List<Position> initialPositions) {
        this.name = name;
        this.map = map;
        this.numAgents = initialPositions.size();
        this.initialPositions = initialPositions;
        width = map.length;
        height = map[0].length;

        for(int x = 0 ; x < width ; x++) {
            for(int y = 0 ; y < height ; y++) {
                if(map[x][y] == OBSTACLE) obstacles.add(new Position(x, y));
                else if(map[x][y] == DEPOT) depots.add(new Position(x, y));
                else if(map[x][y] > 0) {
                    golds.put(new Position(x, y), map[x][y]);
                    goldCount += map[x][y];
                }
            }
        }
    }

    public boolean isObstacle(int x, int y) {
        if(outside(x, y)) return false;
        return map[x][y] == OBSTACLE;
    }
    public boolean isDepot(int x, int y) {
        if(outside(x, y)) return false;
        return map[x][y] == DEPOT;
    }
    public boolean isGold(int x, int y) {
        if(outside(x, y)) return false;
        return map[x][y] > 0;
    }
    public int numGolds(int x, int y) {
        if(outside(x, y)) return 0;
        return Math.max(0, map[x][y]);
    }

    public List<Position> getDepots() {
        return depots;
    }

    public List<Position> getObstacles() {
        return obstacles;
    }

    public Set<HashMap.Entry<Position, Integer>> getGolds() {
        return golds.entrySet();
    }

    public List<Position> getInitialPositions() {
        return initialPositions;
    }

    public boolean pick(int x, int y) {
        if(isGold(x, y)) {
            map[x][y]--;
            goldCount--;
            if(guiEnabled) {
                Position key = new Position(x, y);
                if (map[x][y] == 0) {
                    golds.remove(key);
                } else {
                    golds.put(key, map[x][y]);
                }
            }
            return true;
        } else {
            return false;
        }
    }

    public void drop(int x, int y) {
        if(!isDepot(x, y)) {
            map[x][y]++;
            goldCount++;
            if(guiEnabled) {
                golds.put(new Position(x, y), map[x][y]);
            }
        }
    }

    public void addGold(int x, int y) {
        if(map[x][y] >= 0) {
            map[x][y]++;
            goldCount++;
            golds.put(new Position(x, y), map[x][y]);
        }
    }

    private boolean outside(int x, int y) {
        return x < 0 || x >= width || y < 0 || y >= height;
    }

    public String getName() {
        return name;
    }

    public static MapConfig load(String name, InputStream is) throws IOException {
        BufferedReader in = new BufferedReader(new InputStreamReader(is));
        ArrayList<String> lines = new ArrayList<>();
        ArrayList<Position> agents = new ArrayList<>();
        int width = 0;
        String line;
        while((line = in.readLine()) != null) {
            lines.add(line);
            width = Math.max(width, line.length());
        }

        int[][] map = new int[width][lines.size()];
        for(int y = 0 ; y < lines.size() ; y++) {
            line = lines.get(y);
            for(int x = 0 ; x < line.length() ; x++) {
                char chr = line.charAt(x);
                if(chr >= '1' && chr <= '9') map[x][y] = chr - '0';
                else if(chr == 'D') map[x][y] = DEPOT;
                else if(chr == 'O') map[x][y] = OBSTACLE;
                else if(chr == 'A') {
                    agents.add(new Position(x, y));
                }
            }
        }

        Collections.shuffle(agents);

        return new MapConfig(name, map, agents);
    }

    @Override
    public int getAgentsCount() {
        return numAgents;
    }
}

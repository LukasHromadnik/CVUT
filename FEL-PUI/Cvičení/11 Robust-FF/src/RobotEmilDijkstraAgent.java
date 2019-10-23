import javax.vecmath.Point2i;
import java.util.*;

public class RobotEmilDijkstraAgent extends RobotEmilAgent{

    Point2i nextState=null;
    Point2i goldState=null;
    List<Point2i> currentPlan;
    // First dimension is columns, second dimension is rows.
    List<Point2i> DIRECTIONS = Arrays.asList(new Point2i(0, -1), new Point2i(0, 1), new Point2i(1, 0), new Point2i(-1, 0)); //Order matches order of actions, N, S, E, W



    public Action nextStep(int currentX, int currentY, CellContent[][] map, int iteration) throws PlanFailure {

        Point2i currentState = new Point2i(currentX, currentY);

        // generate initial plan
        if(nextState==null) {
            // Get gold coordinates
            for(int x=0; x<map.length; x++){
                for(int y=0; y<map[0].length; y++){
                    if(map[x][y]==CellContent.GOLD){
                        goldState = new Point2i(x,y);
                    }
                }
            }
            currentPlan = plan(currentX, currentY, goldState.getX(), goldState.getY(), map, iteration);
            nextState = currentState;
        }


        // Replan if Emil is in unexpected state
        if(!nextState.equals(currentState)){
            System.out.println("Replanning now!");
            // currentPlan = plan(currentX, currentY, goldState.getX(), goldState.getY(), map);
            throw new PlanFailure("Unplanned for deviation from the plan!");
        }


        // Get next state according to the plan
        nextState = currentPlan.get(currentPlan.indexOf(currentState)+1);

        return getAction(currentState, nextState);
    }

    public void resetAgent() {
        nextState=null;
        goldState=null;
        currentPlan=null;
    }

    private Action getAction(Point2i current, Point2i next){
        Point2i direction = new Point2i();
        direction.sub(next, current);
        return Action.values()[DIRECTIONS.indexOf(direction)];
    }


    private List<Point2i> plan(int startX, int startY, int destX, int destY, CellContent[][] map, int iteration) {
        Graph graph = new Graph(map.length, map[0].length);

        List<Point2i> plan = new LinkedList<>();

        PriorityQueue<Node> queue = new PriorityQueue<>((o1, o2) -> o1.getDist() - o2.getDist());

        Node start = graph.getNode(startX, startY);
        queue.add(start);
        start.setDist(0);

        Node destination = graph.getNode(destX, destY);

        Node currentNode;
        Node neighbour;

        while (queue.size() > 0 && !destination.isClosed()) {
            currentNode = queue.remove();

            for (Point2i direction : DIRECTIONS) {
                Point2i neighbourCoord = new Point2i();
                neighbourCoord.add(currentNode, direction);

                if(graph.containsCoord(neighbourCoord) &&
                        map[neighbourCoord.getX()][neighbourCoord.getY()] != CellContent.OBSTACLE){
                    neighbour = graph.getNode(neighbourCoord);
                    if(!neighbour.isClosed()){
                        queue.remove(neighbour);
                        neighbour.setDist(Math.min(currentNode.getDist()+1, neighbour.getDist()));
                        neighbour.setParent(currentNode);
                        queue.add(neighbour);
                    }
                }
            }
            currentNode.close();
        }

        currentNode = destination;
        while(currentNode.getParent()!=currentNode){
            plan.add(currentNode);
            currentNode = currentNode.parent;
        }
        Collections.reverse(plan);
        return plan;


    }

    private class Node extends Point2i{
        boolean closed;
        int dist;
        Node parent;

        public Node(int x, int y, int dist){
            super(x, y);
            this.dist = dist;
            this.closed = false;
            this.parent=this;
        }

        public void close(){
            closed=true;
        }

        public boolean isClosed() {
            return closed;
        }

        public int getDist() {
            return dist;
        }

        public void setDist(int dist) {
            this.dist = dist;
        }

        public void setParent(Node parent){
            this.parent = parent;
        }

        public Node getParent() {
            return parent;
        }
    }

    private class Graph{
        Node[][] graph ;
        public Graph(int sizeX,  int sizeY){
            graph = new Node[sizeX][sizeY];
            for(int x=0; x<sizeX; x++){
                for(int y=0; y<sizeY; y++){
                    graph[x][y] =  new Node(x, y, Integer.MAX_VALUE);
                }
            }
        }

        public Node getNode(int x, int y){
            return graph[x][y];
        }

        public Node getNode(Point2i coord){
            return graph[coord.getX()][coord.getY()];
        }

        public int getNodeDist(int x, int y){
            return getNode(x,y).getDist();
        }

        public boolean containsCoord(Point2i coord){
            return coord.getX() >= 0 && coord.getX() < graph.length &&
                    coord.getY() >= 0 && coord.getY() < graph[0].length;
        }
    }


}

package bi.zum.lab1;

import cz.cvut.fit.zum.api.AbstractAlgorithm;
import java.util.HashSet;
import java.util.LinkedList;
import java.util.List;
import cz.cvut.fit.zum.api.Node;
import cz.cvut.fit.zum.api.UninformedSearch;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Map;
import org.openide.util.lookup.ServiceProvider;

/**
 * Depth-first search
 *
 * @see http://en.wikipedia.org/wiki/Depth-first_search
 */
@ServiceProvider(service = AbstractAlgorithm.class, position = 10)
public class DFS extends AbstractAlgorithm implements UninformedSearch {

    private LinkedList<Node> opened;
    private HashSet<Node> closed;
    private Map<Node, Node> prev;
    private List<Node> path;

    @Override
    public String getName() {
        return "DFS";
    }

    @Override
    public List<Node> findPath(Node startNode) {
        opened = new LinkedList<Node>();
        closed = new HashSet<Node>();
        prev = new HashMap<Node, Node>();
        Node current;
        path = null;
        
        opened.push(startNode);
        
        while ( ! opened.isEmpty()) {

            current = opened.pop();

            if (current.isTarget()) {
                return buildPath(startNode, current);
            }

            for (Node y : current.expand()) {
                if ( ! opened.contains(y) && ! closed.contains(y)) {
                    opened.push(y);
                    prev.put(y, current);
                }
            }
            closed.add(current);

        }

        return path;
    }

    private List<Node> buildPath(Node start, Node target) {
        path = new ArrayList<Node>();
        path.add(target);
        
        target = prev.get(target);
        
        while (target != start) {
            path.add(target);
            target = prev.get(target);
        }
        
        return path;
    }
}

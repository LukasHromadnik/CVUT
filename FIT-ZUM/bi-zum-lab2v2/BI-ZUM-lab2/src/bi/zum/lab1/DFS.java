package bi.zum.lab1;

import cz.cvut.fit.zum.api.AbstractAlgorithm;
import cz.cvut.fit.zum.api.Node;
import cz.cvut.fit.zum.api.UninformedSearch;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.HashSet;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;
import java.util.Set;
import org.openide.util.lookup.ServiceProvider;

/**
 * Depth-first search
 *
 * @see http://en.wikipedia.org/wiki/Depth-first_search
 */
@ServiceProvider(service = AbstractAlgorithm.class)
public class DFS extends PathRecSearch implements UninformedSearch {

    private HashSet<Node> closed;
    private LinkedList<Node> opened;
    
    @Override
    public String getName() {
        return "DFS";
    }

    @Override
    public List<Node> findPath(Node startNode) {
        opened = new LinkedList<Node>();
        closed = new HashSet<Node>();
        prev = new HashMap<Node, Node>();
        
        opened.add(startNode);

        while (!opened.isEmpty()) {

            // this is the first of two lines you were to modify in Lab#1
            Node current = opened.pollLast();

            if (current.isTarget()) {
                return buildPath(current);
            }

            for (Node y : current.expand()) {
                if (!opened.contains(y) && !closed.contains(y)) {
                    opened.addLast(y);
                    
                    // this is the second line you were to modify in Lab#1
                    prev.put(y, current);
                }
            }
            closed.add(current);
        }

        return null;
    }
}

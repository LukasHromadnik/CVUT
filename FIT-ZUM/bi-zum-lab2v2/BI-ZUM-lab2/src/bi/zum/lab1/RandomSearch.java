package bi.zum.lab1;

import cz.cvut.fit.zum.api.AbstractAlgorithm;
import cz.cvut.fit.zum.api.Node;
import cz.cvut.fit.zum.api.UninformedSearch;
import java.util.Collections;
import java.util.HashMap;
import java.util.HashSet;
import java.util.LinkedList;
import java.util.List;
import org.openide.util.lookup.ServiceProvider;

/**
 * WARNINIG: this is very stupid algorithm!!!
 *
 * Should serve only as an example of UninformedSearch usage
 *
 * @author Tomas Barton
 */
@ServiceProvider(service = AbstractAlgorithm.class, position = 2)
public class RandomSearch extends PathRecSearch implements UninformedSearch {

    private HashSet<Node> closed;
    private List<Node> opened;

    @Override
    public String getName() {
        return "random search";
    }

    @Override
    public List<Node> findPath(Node startNode) {
        opened = Collections.synchronizedList(new LinkedList<Node>());
        closed = new HashSet<Node>();
        prev = new HashMap<Node, Node>();
        
        opened.add(startNode);

        while (!opened.isEmpty()) {

            Node current = random(opened);

            if (current.isTarget()) {
                return buildPath(current);
            }

            for (Node y : current.expand()) {
                if (!opened.contains(y) && !closed.contains(y)) {
                    opened.add(y);
                    prev.put(y, current);
                }
            }
            closed.add(current);
        }

        return null;
    }

    /**
     * Select random node from list
     *
     * @param list
     * @return Node
     */
    private Node random(List<Node> list) {
        int min = 0;
        int max = list.size();

        if (max == 1) {
            return list.remove(0);
        }
        int num = min + (int) (Math.random() * ((max - min)));

        //we want to remove explored nodes
        return list.remove(num);
    }
}

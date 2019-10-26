package bi.zum.lab1;

import cz.cvut.fit.zum.api.AbstractAlgorithm;
import java.util.HashSet;
import java.util.LinkedList;
import java.util.List;
import cz.cvut.fit.zum.api.Node;
import cz.cvut.fit.zum.api.UninformedSearch;
import java.util.ArrayList;
import java.util.Collections;
import java.util.HashMap;
import java.util.Map;
import java.util.Set;
import org.openide.util.lookup.ServiceProvider;

/**
 * Breadth-first search
 *
 *
 * @see http://en.wikipedia.org/wiki/Breadth-first_search
 *
1  procedure BFS(G,v):
2      create a queue Q
3      enqueue v onto Q
4      mark v
5      while Q is not empty:
6          t ← Q.dequeue()
7          if t is what we are looking for:
8              return t
9          for all edges e in G.adjacentEdges(t) do
12             u ← G.adjacentVertex(t,e)
13             if u is not marked:
14                  mark u
15                  enqueue u onto Q
16     return none

 *
 */
@ServiceProvider(service = AbstractAlgorithm.class)
public class BFS extends PathRecSearch implements UninformedSearch {

    private HashSet<Node> closed;
    private LinkedList<Node> opened;

    
    @Override
    public String getName() {
        return "BFS";
    }

    /**
     * When implementing uninformed search the only way how to find out whether
     * you reached your destination is calling node.isTarget() on each Node you
     * find. When you've checked a Node you should add it to closed list, so
     * that you'll avoid infinite loops in your program.
     *
     * @param startNode starting Node is represented by red color on the
     * @return List of Nodes which represents the shortest path from targetNode
     * (which you have to find first) to startNode
     */
    @Override
    public List<Node> findPath(Node startNode) {
        opened = new LinkedList<Node>();
        closed = new HashSet<Node>();
        prev = new HashMap<Node, Node>();
        
        opened.add(startNode);

        while (!opened.isEmpty()) {

            // this is the first of two lines you were to modify in Lab#1
            Node current = opened.pollFirst();

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
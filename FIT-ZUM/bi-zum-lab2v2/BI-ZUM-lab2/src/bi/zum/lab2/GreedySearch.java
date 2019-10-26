package bi.zum.lab2;

import bi.zum.lab1.PathRecSearch;
import bi.zum.lab2.util.Euclidean;
import bi.zum.lab2.util.ZumPriorityQueue;
import cz.cvut.fit.zum.api.AbstractAlgorithm;
import cz.cvut.fit.zum.api.InformedSearch;
import cz.cvut.fit.zum.api.Node;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import org.openide.util.lookup.ServiceProvider;

/**
 *
 * @author Tomáš Řehořek
 */
@ServiceProvider(service = AbstractAlgorithm.class)
public class GreedySearch extends PathRecSearch implements InformedSearch {
    
    private ZumPriorityQueue<Node> open;
    private HashSet<Node> closed;
    
    @Override
    public String getName() {
        return "Greedy search";
    }
    
    @Override
    public List<Node> findPath(Node startNode, Node endNode){
        
        open = new ZumPriorityQueue<Node>();
        closed = new HashSet<Node>();
        prev = new HashMap<Node, Node>();
        
        double d;
        
        
        Map<Node, Double> dist = new HashMap<Node, Double>();
        
        open.enqueue(startNode, 0);
        dist.put(startNode, 0.0);
        
        while (!open.isEmpty()) {

            // this is the first of two lines you were to modify in Lab#1
            Node x = open.dequeue();

            if (x.isTarget()) {
                return buildPath(x);
            }
            
            for (Node y : x.expand()) {
                if ( ! closed.contains(y)) {
                    d = Euclidean.distance(y, x) + dist.get(x);
                    
                    dist.put(y, d);
                    prev.put(y, x);
                    
                    if ( ! open.contains(y))
                        open.enqueue(y, Euclidean.distance(y, endNode));
       
                }
            }
            closed.add(x);
        }           
        
        return null;
    }
        
}

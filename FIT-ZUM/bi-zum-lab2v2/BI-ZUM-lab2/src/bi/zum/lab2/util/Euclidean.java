package bi.zum.lab2.util;

import cz.cvut.fit.zum.api.Node;

/**
 * Utility class for calculating euclidean distance. Declares a static method
 * for calculating the distance, so the following code may be used in you
 * implementation:
* <pre>
* {@code
* double distance = Euclidean.distance(a, b);
* }
* </pre>
 * @author Tomáš Řehořek
 */
public class Euclidean {

    public static double distance(Node a, Node b) {
        
        return Math.sqrt(Math.pow(a.getX() - b.getX(), 2) + Math.pow(a.getY() - b.getY(), 2));
    }
}

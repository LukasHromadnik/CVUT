package bi.zum.lab1;

import cz.cvut.fit.zum.ZumGraphFrame;
import javax.swing.SwingUtilities;
import javax.swing.UIManager;
import javax.swing.WindowConstants;
import org.openide.util.Exceptions;

/**
 * No need to change anything in here. If you want to implement your own
 * algorithm, make sure that you add the full class name to the file 
 * 
 * META-INF/services/cz.cvut.fit.zum.api.AbstractAlgorithm
 *
 * @author Tomas Barton
 */
public class BIZUMlab1 {

    /**
     * @param args the command line arguments
     */
    public static void main(String[] args) {
        try {
            UIManager.setLookAndFeel(UIManager.getSystemLookAndFeelClassName());
        } catch (Exception e) {
            Exceptions.printStackTrace(e);
        }

        SwingUtilities.invokeLater(new Runnable() {
            @Override
            public void run() {
                try {
                    ZumGraphFrame app = new ZumGraphFrame();
                    app.setSize(800, 860);
                    app.setVisible(true);
                    app.setDefaultCloseOperation(WindowConstants.EXIT_ON_CLOSE);

                } catch (Exception e) {
                    Exceptions.printStackTrace(e);
                }
            }
        });
    }
}

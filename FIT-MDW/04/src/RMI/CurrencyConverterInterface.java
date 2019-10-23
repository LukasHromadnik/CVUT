/**
 * Created by Lukáš Hromadník on 13.11.16.
 */

package RMI;

import java.rmi.Remote;
import java.rmi.RemoteException;

public interface CurrencyConverterInterface extends Remote {
    public double convert(Currency from, Currency to, double amount) throws RemoteException;;
}

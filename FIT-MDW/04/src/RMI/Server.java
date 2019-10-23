/**
 * Created by Lukáš Hromadník on 13.11.16.
 */

package RMI;

import java.rmi.Naming;
import java.rmi.RemoteException;
import java.rmi.registry.LocateRegistry;
import java.rmi.server.UnicastRemoteObject;

public class Server extends UnicastRemoteObject implements CurrencyConverterInterface {

    private static final long serialVersionUID = 1L;

    protected Server() throws RemoteException {
        super();
    }

    @Override
    public double convert(Currency from, Currency to, double amount) {
        return Currency.getExchangeRates().get(from).get(to) * amount;
    }

    public static void main(String[] args) {
        try {
            LocateRegistry.createRegistry(1099);

            Server server = new Server();
            Naming.rebind("//localhost/Hello", server);

            System.out.println("RMI.Server started...");

        } catch (Exception e) {
            System.out.println("Error: " + e.getLocalizedMessage());
        }
    }
}

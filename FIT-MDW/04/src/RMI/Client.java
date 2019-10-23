/**
 * Created by Lukáš Hromadník on 13.11.16.
 */

package RMI;

import java.rmi.Naming;
import java.util.ArrayList;

public class Client {

    public static void main(String[] args) throws Exception {
        CurrencyConverterInterface client = (CurrencyConverterInterface) Naming.lookup("//localhost/Hello");

        ArrayList<CurrencyConvert> data = new ArrayList<CurrencyConvert>();
        data.add(new CurrencyConvert(Currency.CZK, Currency.GBP, 87.0));
        data.add(new CurrencyConvert(Currency.CZK, Currency.EUR, 1.0));
        data.add(new CurrencyConvert(Currency.GBP, Currency.USD, 35.0));
        data.add(new CurrencyConvert(Currency.GBP, Currency.CZK, 10.0));
        data.add(new CurrencyConvert(Currency.EUR, Currency.USD, 95.0));
        data.add(new CurrencyConvert(Currency.EUR, Currency.GBP, 10.0));
        data.add(new CurrencyConvert(Currency.USD, Currency.CZK, 284.0));
        data.add(new CurrencyConvert(Currency.USD, Currency.EUR, 49.0));

        for (CurrencyConvert item : data) {
            double convert = client.convert(item.from, item.to, item.amount);
            System.out.println(item.amount + " " + item.from + " = " + convert + " " + item.to);
        }
    }

}

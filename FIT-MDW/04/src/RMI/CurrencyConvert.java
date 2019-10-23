/**
 * Created by Lukáš Hromadník on 13.11.16.
 */

package RMI;

public class CurrencyConvert {

    public Currency from;
    public Currency to;
    public double amount;

    public CurrencyConvert(Currency from, Currency to, double amount) {
        this.from = from;
        this.to = to;
        this.amount = amount;
    }
}

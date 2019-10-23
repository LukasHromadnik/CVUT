/**
 * Created by Lukáš Hromadník on 13.11.16.
 */

package RMI;

import java.util.EnumMap;

public enum Currency {
    CZK, EUR, GBP, USD;

    public static EnumMap<Currency, EnumMap<Currency, Double>> getExchangeRates() {
        EnumMap<Currency, EnumMap<Currency, Double>> data = new EnumMap<Currency, EnumMap<Currency, Double>>(Currency.class);

        EnumMap<Currency, Double> czk = new EnumMap<Currency, Double>(Currency.class);
        czk.put(Currency.USD, 0.040319);
        czk.put(Currency.EUR, 0.036962);
        czk.put(Currency.GBP, 0.031844);
        data.put(Currency.CZK, czk);

        EnumMap<Currency, Double> eur = new EnumMap<Currency, Double>(Currency.class);
        eur.put(Currency.USD, 1.091);
        eur.put(Currency.CZK, 27.055);
        eur.put(Currency.GBP, 0.861542);
        data.put(Currency.EUR, eur);

        EnumMap<Currency, Double> gbp = new EnumMap<Currency, Double>(Currency.class);
        gbp.put(Currency.USD, 1.266);
        gbp.put(Currency.CZK, 31.403);
        gbp.put(Currency.EUR, 1.161);
        data.put(Currency.GBP, gbp);

        EnumMap<Currency, Double> usd = new EnumMap<Currency, Double>(Currency.class);
        usd.put(Currency.GBP, 0.789797);
        usd.put(Currency.CZK, 24.802);
        usd.put(Currency.EUR, 0.916725);
        data.put(Currency.USD, usd);

        return data;
    }
}

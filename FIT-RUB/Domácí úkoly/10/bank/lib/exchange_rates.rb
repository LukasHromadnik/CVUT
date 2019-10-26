#!/usr/bin/env ruby
require 'open-uri'
require 'bigdecimal'

# Loader for exchange rates
class ExchangeRates
  USER_AGENT = 'Mozilla/5.0 (Windows NT 6.1)'
  PAGE_URI = 'http://www.cnb.cz/cs/financni_trhy/devizovy_trh/kurzy_devizoveho_trhu/denni_kurz.txt'

  @rates = { CZK: BigDecimal.new('1.0') }
  open(PAGE_URI, 'User-Agent' => USER_AGENT) do |f|
    lines_read = 0
    f.each_line do |line|
      lines_read += 1
      next if lines_read <= 2
      parts = line.split('|')
      currency = parts[3].to_sym
      rate = parts[4].sub(',', '.')
      @rates[currency] = BigDecimal.new(rate)
    end
  end

  def self.rates
    @rates
  end

  def self.supported?(currency)
    @rates.key?(currency)
  end

  def self.change(value, currency_from, currency_to)
    # neprevadime nahodou .. zbytecne?
    return value if currency_from == :CZK && currency_to == :CZK

    # prevadet lze libovolne meny
    if currency_from != :CZK && currency_to != :CZK
      value = change(value, currency_from, :CZK)
      currency_from = :CZK
    end

    # ted se prevadi do nebo z CZK
    if currency_from == :CZK
      value / @rates[currency_to]
    else
      value * @rates[currency_from]
    end
  end
end

#!/usr/bin/env ruby
require_relative 'account'
require_relative 'exchange_rates'
require_relative 'money'
require_relative 'transaction'

# Bank API
class Bank
  def initialize
    @accounts = []
    @transactions = []
  end

  def open_account
    @accounts << Account.new
    @accounts.last
  end

  def balance(account)
    account.balance
  end

  def send(source_account, dest_account, money)
    source_account.withdraw(money)
    dest_account.deposit(money)
    @transactions << Transaction.new(source_account, dest_account, money)
  end

  def transactions(account)
    res = []
    @transactions.each do |t|
      res << t if t.source_account == account || t.dest_account == account
    end
    res
  end

  def exchange(value, currency_from, currency_to)
    ExchangeRates.change(value, currency_from, currency_to)
  end
end

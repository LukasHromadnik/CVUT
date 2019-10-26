#!/usr/bin/env ruby
require_relative 'exchange_rates'
require_relative 'insufficient_deposit_error'

# Bank Account
class Account
  attr_reader :id, :balance
  @ids = 0

  class << self
    attr_accessor :ids
  end

  def initialize
    @balance = Money.new(0, :CZK)
    Account.ids += 1
    @id = Account.ids
  end

  def deposit(money)
    @balance += money
  end

  def withdraw(m)
    fail InsufficientDepositError, 'Not enough money' unless at_least?(m)
    @balance -= m
  end

  def at_least?(money)
    @balance >= money
  end
end

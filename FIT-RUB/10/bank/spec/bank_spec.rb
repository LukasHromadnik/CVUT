#!/usr/bin/env ruby
require_relative 'spec_helper'
require_relative '../lib/bank'

describe Bank do
  subject(:bank) { described_class.new }

  context 'account' do
    it 'allows to opening new account' do
      expect(bank.respond_to?(:open_account)).to eq true
      expect(bank.open_account).to be_instance_of(Account)
    end

    it 'allows to check account balance' do
      account = bank.open_account
      expect(bank.balance(account)).to eq account.balance
    end
  end

  context 'transaction' do
    it 'allows money transactions' do
      acc1 = bank.open_account
      acc2 = bank.open_account
      acc1.deposit(Money.new(1000, :USD))
      expect(acc1.balance).to eq Money.new(1000, :USD)
      bank.send(acc1, acc2, Money.new(200, :USD))
      expect(acc1.balance).to eq Money.new(800, :USD)
      expect(acc2.balance).to eq Money.new(200, :USD)
    end

    it 'does not allow transfer if account money amount is not high enough' do
      acc1 = bank.open_account
      acc2 = bank.open_account
      acc1.deposit(Money.new(1, :CZK))
      expect do
        bank.send(acc1, acc2, Money.new(200, :USD))
      end.to raise_error InsufficientDepositError
    end

    it 'keeps records of executed transactions' do
      acc1 = bank.open_account
      acc2 = bank.open_account
      acc1.deposit(Money.new(1000, :CZK))
      acc2.deposit(Money.new(1000, :CZK))
      bank.send(acc1, acc2, Money.new(50, :CZK))
      bank.send(acc1, acc2, Money.new(150, :CZK))
      bank.send(acc2, acc1, Money.new(100, :CZK))

      expect(acc1.balance).to eq Money.new(900, :CZK)
      expect(acc2.balance).to eq Money.new(1100, :CZK)

      expect(bank.transactions(acc1).size).to eq 3
      expect(bank.transactions(acc2).size).to eq 3
    end

    it 'should exchange money in specified currency' do
      five_dollars = Money.new(5, :USD)
      five_dollars_in_czk = five_dollars.to_czk
      five_dollars_in_czk_raw = bank.exchange(5, :USD, :CZK)
      expect(five_dollars_in_czk.value).to eq five_dollars_in_czk_raw
    end
  end
end

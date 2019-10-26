#!/usr/bin/env ruby
require_relative '../duck'
require_relative 'spec_helper'

describe 'Duck' do

  before(:each) do
    @duck = Duck.new('Fanny', 2)
  end

  context 'object' do
    subject { Duck.new('Fanny', 2) }
    it { is_expected.to be_an_instance_of Duck }
  end

  context 'has name Fanny' do
    it { expect(@duck.name).to eq 'Fanny' }
  end

  context 'flying' do
    it 'does not fly unless told to' do
      expect(@duck.flying?).to be false
    end

    it { expect(Duck.can_fly).to be true }

    it 'does fly' do
      @duck.flying = true
      expect(@duck.flying?).to be true
    end

    it 'shouldn\'t swim while it is flying' do
      @duck.swimming = true
      @duck.flying = true
      expect(@duck.flying?).to be true
      expect(@duck.swimming?).to be false
    end
  end

  context 'swimming' do
    it { expect(Duck.can_swim).to be true }

    it 'should not swim unless told to do that' do
      expect(@duck.swimming?).to be false
    end

    it 'does swim' do
      @duck.swimming = true
      expect(@duck.swimming?).to eq true
    end

    it 'shouldn\'t fly while it is swimming' do
      @duck.flying = true
      expect(@duck.flying?).to eq true
      @duck.swimming = true
      expect(@duck.flying?).to eq false
      expect(@duck.swimming?).to eq true
    end
  end

  context 'quacking' do
    it { expect(@duck.quack).to eq 'quack quack' }

    it 'counts number of quacks' do
      @duck.quack
      expect(@duck.num_quacks).to eq 2
      @duck.quack
      expect(@duck.num_quacks).to eq 4
    end

    it 'should count total number of quacks for all Ducks' do
      donald = Duck.new('Donald', 3)
      donald.quack
      expect(Duck.num_quacks).to eq 8
    end
  end

  context 'private methods' do
    it 'consider age as a private matter!' do
      expect { @duck.age }.to raise_error NoMethodError
    end

    it 'does not increase number of quack without a quack' do
      expect { @duck.quack = 15 }.to raise_error NoMethodError
      expect { @duck.num_quacks = 15 }.to raise_error NoMethodError
    end

    it 'should not increase global number of quack without a quack' do
      expect { Duck.num_quacks = 15 }.to raise_error NoMethodError
    end
  end

  context 'age' do
    it 'should compare which duck is older' do
      old_duck = Duck.new 'old one', 15
      expect(@duck.compare(old_duck)).to eq(-1)
      same_duck = Duck.new 'same age', 2
      expect(@duck.compare(same_duck)).to eq 0
      young_duck = Duck.new 'young duck', 1
      expect(@duck.compare(young_duck)).to eq 1
    end
  end

  context 'imperative' do
    it 'should have fly! method' do
      @duck.fly!
      expect(@duck.flying?).to eq true
      expect(@duck.swimming?).to eq false
    end

    it 'fly! should use flying= method' do
      @duck.swimming = true
      @duck.fly!
      expect(@duck.flying?).to eq true
      expect(@duck.swimming?).to eq false
    end
  end
end

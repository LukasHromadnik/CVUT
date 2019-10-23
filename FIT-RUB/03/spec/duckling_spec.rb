#!/usr/bin/env ruby
require_relative '../duckling'
require_relative 'spec_helper'

describe 'Duckling' do

  before(:each) do
    @duckling = Duckling.new('Fanny', 2)
  end

  context 'has name Fanny' do
    it { expect(@duckling.name).to eq 'Fanny' }
  end

  context 'flying' do
    it 'does not fly' do
      expect(@duckling.flying?).to be false
    end

    it { expect(Duckling.can_fly).to be false }

    it 'does not fly' do
      @duckling.flying = true
      expect(@duckling.flying?).to be false
    end

    it 'should swim cause it is unable to fly' do
      @duckling.swimming = true
      @duckling.flying = true
      expect(@duckling.flying?).to be false
      expect(@duckling.swimming?).to be true
    end
  end

  context 'swimming' do
    it { expect(Duckling.can_swim).to be true }

    it 'should not swim unless told to do that' do
      expect(@duckling.swimming?).to be false
    end

    it 'does swim' do
      @duckling.swimming = true
      expect(@duckling.swimming?).to eq true
    end

    it 'shouldn\'t fly (cause it can\'t) while it is swimming' do
      @duckling.flying = true
      expect(@duckling.flying?).to eq false
      @duckling.swimming = true
      expect(@duckling.flying?).to eq false
      expect(@duckling.swimming?).to eq true
    end
  end

  context 'quacking' do
    it { expect(@duckling.quack).to eq 'quack quack' }

    it 'counts number of quacks' do
      @duckling.quack
      expect(@duckling.num_quacks).to eq 2
      @duckling.quack
      expect(@duckling.num_quacks).to eq 4
    end

    it 'should count total number of quacks for all Ducks' do
      donald = Duckling.new('Donald', 3)
      donald.quack
      expect(Duckling.num_quacks).to eq 16
    end
  end

  context 'private methods' do
    it 'consider age as a private matter!' do
      expect { @duckling.age }.to raise_error NoMethodError
    end

    it 'does not increase number of quack without a quack' do
      expect { @duckling.quack = 15 }.to raise_error NoMethodError
      expect { @duckling.num_quacks = 15 }.to raise_error NoMethodError
    end

    it 'should not increase global number of quack without a quack' do
      expect { Duckling.num_quacks = 15 }.to raise_error NoMethodError
    end
  end

  context 'age' do
    it 'should compare which duck is older' do
      old_duck = Duckling.new 'old one', 15
      expect(@duckling.compare(old_duck)).to eq(-1)
      same_duck = Duckling.new 'same age', 2
      expect(@duckling.compare(same_duck)).to eq 0
      young_duck = Duckling.new 'young duck', 1
      expect(@duckling.compare(young_duck)).to eq 1
    end
  end

  context 'imperative' do
    it 'should have fly! method but it does nothing' do
      @duckling.fly!
      expect(@duckling.flying?).to eq false
    end

    it 'fly! should use flying= method' do
      @duckling.swimming = true
      @duckling.fly!
      expect(@duckling.flying?).to eq false
      expect(@duckling.swimming?).to eq true
    end
  end
end

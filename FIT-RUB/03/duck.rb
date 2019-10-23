#!/usr/bin/env ruby

class Duck
  attr_accessor :name, :age, :flying, :swimming
  attr_reader :quack, :num_quacks
  protected :age

  @@num_quacks = 0

  def initialize(name, age)
    @name = name
    @age = age
    @flying = false
    @swimming = false
    @num_quacks = 0
  end

  def flying=(flying)
    @flying = flying && self.class.can_fly
    if @flying
      @swimming = false
    end
  end

  def swimming=(swimming)
    @swimming = swimming
    if @swimming
      @flying = false
    end
  end

  def flying?
    @flying
  end

  def self.can_fly
    true
  end

  def swimming?
    @swimming && !@flying
  end

  def self.can_swim
    true
  end

  def quack
    @num_quacks += 2
    @@num_quacks += 2
    'quack quack'
  end
  
  def self.num_quacks
    @@num_quacks
  end

  def compare(duck)
    @age <=> duck.age
  end

  def fly!
    self.flying = true
  end

end

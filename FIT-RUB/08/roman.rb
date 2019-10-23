#!/usr/bin/env ruby

class Roman
  attr_accessor :value
  MAX_ROMAN = 4999
  FACTORS = [['m', 1000], ['cm', 900], ['d', 500], ['cd', 400],
  ['c', 100], ['xc', 90], ['l', 50], ['xl', 40],
  ['x', 10], ['ix', 9], ['v', 5], ['iv', 4],
  ['i', 1]]

  def initialize(value)
    if value <= 0 || value > MAX_ROMAN
      fail 'Roman values must be > 0 and <= #{MAX_ROMAN}'
    end
    @value = value
  end

  def to_s
    value = @value
    roman = ''
    until value.zero? do
      factor = FACTORS.select { |code, number| number <= value }.first
      value -= factor[1]
      roman << factor[0]
    end
    roman
  end

  alias :to_i :value
  alias :to_int :value

  def +(other)
    @value + other.to_i
  end

  def ==(other)
    @value == other.to_i
  end

  def coerce(other)
    if Float === other then
      return other, Float(@value)
    end
    return Roman.new(other), self
  end

  def *(other)
    @value * other.to_i
  end

  def -(other)
    @value - other.to_i
  end

  def /(other)
    @value / other.to_i
  end

  def >(other)
    @value > other.to_i
  end

  def <(other)
    @value < other.to_i
  end

  def ==(other)
    if other.instance_of?(Float)
      return other == Float(@value)
    end

    return @value == other.to_i
  end

  def !=(other)
    @value != other.to_i
  end

  def <=(other)
    @value <= other.to_i
  end

  def >=(other)
    @value >= other.to_i
  end

  def >=(other)
    @value >= other.to_i
  end

  def between?(first, second)
    @value.between? first.to_i, second.to_i
  end

  def <=>(other)
    @value <=> other.to_i
  end

  def eql?(other)
    if !other.instance_of? Roman then
      return false
    end
    
    return @value.eql?(other.to_i)
  end

  def equal?(other)
    object_id === other.object_id
  end

end

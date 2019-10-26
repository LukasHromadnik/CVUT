#!/usr/bin/env ruby

# Immitates some behaviour of the Array class
#
# see: http://www.ruby-doc.org/core/Array.html
class MyArray

  def initialize(data = [])
    @data = data
  end

  def size
    @data.size
  end

  def each(&block)
    if block_given?
      index = 0
      while index < size
        yield @data[index] if block_given?
        index += 1
      end
    else
      self.to_enum
    end
  end

  def reverse
    reversed_array = []
    index = size - 1
    while index >= 0
      reversed_array << @data[index]
      index -= 1
    end

    reversed_array
  end

  def reverse!
    @data = reverse
  end

  def reverse_each(&block)
    if block_given?
      index = size - 1
      while index >= 0
        yield @data[index] if block_given?
        index -= 1
      end
    else
      Enumerator.new do |item|
        index = size - 1
        while index >= 0
          item << @data[index]
          index -= 1
        end
      end
    end
  end

  def pop
    new_data = []

    index = 0
    while index < size - 1
      new_data << @data[index]
      index += 1
    end

    result = @data[index]
    @data = new_data

    result
  end

  def select(&block)
    result = []
    self.each do |item|
      if yield item
        result << item
      end
    end

    result
  end

  def map(&block)
    if block_given?
      index = 0
      result = []
      while index < size
        item = yield @data[index]
        result << item
        index += 1
      end
      result
    else
      self.to_enum
    end
  end

  def to_enum
    Enumerator.new do |item|
      index = 0
      while index < size
        item << @data[index]
        index += 1
      end
    end
  end

  def clear
    @data = []
  end

  def include?(item)
    self.each do |p|
      if p == item
        return true
      end
    end

    return false
  end

  def max(&block)
    index = 0
    max = @data[index]
    index += 1
    while index < size
      if block_given?
        if (yield @data[index], max) == 1
          max = @data[index]
        end
      elsif @data[index] > max
        max = @data[index]
      end

      index += 1
    end

    max
  end
end

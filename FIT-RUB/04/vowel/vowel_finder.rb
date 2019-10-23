#!/usr/bin/env ruby

module Summable
  def sum
    reduce(:+)
  end
end

class Array
  include Summable
end

class VowelFinder
  include Enumerable
  include Summable

  @@vowels = %w(a e i o u y)

  attr_reader :data

  def initialize(data)
    @data = data.downcase
  end

  def each
    valid_data = @data.split("").select { |item| @@vowels.include? item }

    if block_given?
      valid_data.each { |item| yield item }
    else
      valid_data.to_enum
    end
  end

end

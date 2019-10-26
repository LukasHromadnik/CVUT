module MyNumbers
  PI = 3.14

  THE_ANSWER = 42

  class SimpleNumber
    attr_accessor :value

    def initialize(num)
      fail unless num.is_a?(Numeric)
      @x = num
    end

    def add(y)
      @x + y
    end

    def multiply(y)
      @x * y
    end
  end

  # == Example
  # Lorem _ipsum_ dolor *sit amet*, consectetur adipisicing elit, sed
  # do eiusmod tempor incididunt ut labore et dolore magna aliqua.
  # Ut enim ad minim veniam, quis nostrud exercitation ullamco
  # laboris nisi ut aliquip ex ea commodo consequat.
  #
  # * *Args*    :
  #   - +apples+ -> the number of apples
  #   - +oranges+ -> the number of oranges
  #   - +pears+ -> the number of pears
  # * *Returns* :
  #   - the total number of fruit as an integer
  # * *Raises* :
  #   - +ArgumentError+ -> if any value is nil or negative
  #
  def sum_fruit(apples, oranges, pears)
    fail ArgumentError, 'No value can be absent' if apples.nil? || oranges.nil? || pears.nil?
    fail ArgumentError, 'All values must be positive' unless apples >= 0 && oranges >= 0 && pears >= 0

    apples + oranges + pears
  end

  module Roman
    class Roman
      include Comparable

      MAX_ROMAN = 4999
      FACTORS = [['m', 1000], ['cm', 900], ['d', 500], ['cd', 400],
                 ['c', 100], ['xc', 90], ['l', 50], ['xl', 40],
                 ['x', 10], ['ix', 9], ['v', 5], ['iv', 4],
                 ['i', 1]]
      attr_reader :value
    end
  end
end

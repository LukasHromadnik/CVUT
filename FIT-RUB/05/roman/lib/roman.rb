class Roman
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
end

# p Roman.new(2).to_s
class Score
  def initialize
    @font = Gosu::Font.new(20)
    @score = 0
  end

  def draw
    @font.draw("Score: #{@score}", 10, 10, 2, 1.0, 1.0, 0xff_ffff00)
  end

  def update_delta(delta)
  end

  def increase
    @score += 1
  end
end

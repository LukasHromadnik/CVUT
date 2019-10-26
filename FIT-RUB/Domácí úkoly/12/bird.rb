class Bird
  def initialize(x, y, pig, score)
    @x, @y = x, y
    @img = Gosu::Image.new('media/bird.png')
    @pig = pig
    @hidden = false
    @score = score
  end

  def draw
    @img.draw(@x, @y, 1) unless @hidden
  end

  def update_delta(delta)
    if @pig.x >= @x && @pig.y >= @y && @pig.x < (@x + 70) && @pig.y < (@y + 82)
      hide()
    end
  end

  def hide
    unless @hidden
      @hidden = true
      @score.increase
    end
  end
end

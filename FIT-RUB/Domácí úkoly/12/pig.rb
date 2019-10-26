class Pig
  attr_reader :x, :y

  def initialize
    @x, @y = 0, 0
    @pig_anim = Gosu::Image::load_tiles('media/pig1.png', 128, 155)
  end

  def update_delta(delta)
    distance = delta * 100
    if Gosu::button_down? Gosu::KbDown
      @y += distance
    end
    if Gosu::button_down? Gosu::KbUp
      @y -= distance
    end
    if Gosu::button_down? Gosu::KbLeft
      @x -= distance
    end
    if Gosu::button_down? Gosu::KbRight
      @x += distance
    end
  end

  def draw
    @pig_anim[Gosu::milliseconds / 200 % @pig_anim.size].draw(@x, @y, 1)
  end
end

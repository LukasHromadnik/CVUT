# -*- encoding : utf-8 -*-
class TableTennisScorer
  attr_accessor :player1, :player2

  def initialize
    @player1 = TableTennisPlayer.new
    @player2 = TableTennisPlayer.new
  end

  def score
    score1 = @player1.score
    score2 = @player2.score

    if (score1 >= 11 || score2 >= 11) && (score1 - score2).abs >= 2
      score1 > score2 ? 'server game' : 'receiver game'
    else
      @player1.score.to_s + '-' + @player2.score.to_s
    end
  end

end

class TableTennisPlayer
  attr_accessor :score

  def initialize
    @score = 0
  end

  def scores
    @score += 1
  end

end


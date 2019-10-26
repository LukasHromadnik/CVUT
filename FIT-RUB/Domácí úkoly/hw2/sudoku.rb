#!/bin/ruby
require_relative './grid'
require_relative './string_parser'

# Basic sudoku solver
class Sudoku
  PARSERS = [StringParser]

  EXCLUDE = proc do |enum, val|
    enum.each do |e|
      e.exclude(val)
    end
  end

  def initialize(game)
    @grid = load(game)
  end

  # Return true when there is no missing number
  def solved?
    !@grid.nil? && @grid.missing == 0
  end

  # Solves sudoku and returns 2D Grid
  def solve
    fail 'invalid game given' unless @grid.valid?
    puts "missing values #{@grid.missing}, filled #{@grid.filled}"
  end

  protected

  def load(game)
    PARSERS.each do |p|
      return p.load(game) if p.supports?(game)
    end
    fail "input '#{game}' is not supported yet"
  end
end

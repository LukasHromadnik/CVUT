#!/usr/bin/env ruby

$LOAD_PATH << './'
require 'rspec'
require_relative 'spec_helper'
require 'grid'
require 'string_parser'

describe Grid do
  # StringParser should return Grid object with
  # loaded content
  subject(:grid) { StringParser.load(game) }
  let(:game) do
    '003020600900305001001806400008102900700' \
    '000008006708200002609500800203009005010300'
  end
  # to_s output
  #---------+---------+---------
  #       3 |    2    | 6
  # 9       | 3     5 |       1
  #       1 | 8     6 | 4
  #---------+---------+---------
  #       8 | 1     2 | 9
  # 7       |         |       8
  #       6 | 7     8 | 2
  #---------+---------+---------
  #       2 | 6     9 | 5
  # 8       | 2     3 |       9
  #       5 |    1    | 3
  #---------+---------+---------

  describe 'loaded data' do
    it 'has value method for accessing elements' do
      expect(subject.value(0, 0)).to eq 0
    end

    it 'returns row in sudoku grid' do
      expect(subject[0].map(&:to_i)).to eq([0, 0, 3, 0, 2, 0, 6, 0, 0])
      expect(subject[4].map(&:to_i)).to eq([7, 0, 0, 0, 0, 0, 0, 0, 8])
    end

    it 'has [][] method for accessing elements' do
      expect(subject[1, 0].to_i).to eq 9
      expect(subject[2, 3].to_i).to eq 8
      expect(subject[2, 5].to_i).to eq 6
      expect(subject[2, 6].to_i).to eq 4
      expect(subject[4, 0].to_i).to eq 7
      expect(subject[7, 0].to_i).to eq 8
      expect(subject[8, 8].to_i).to eq 0
    end

    it 'has to_s method' do
      expect(subject.respond_to?(:to_s)).to eq true
      res = subject.to_s
      expect(res.class).to eq String
      ary = res.split("\n")
      expect(ary.size).to eq 13
      expect(ary[0]).to eq('---------+---------+---------')
      expect(ary[1]).to eq('       3 |    2    | 6       ')
      expect(ary[0] == ary[12]).to eq true
    end
  end

  describe 'accessing data' do
    let(:game) do
      '4.....8.5.3..........7......2.....6.....8.4'\
      '......1.......6.3.7.5..2.....1.4.....9'
    end

    it 'should return number of not filled elements' do
      expect(subject.missing).to eq 63
    end

    it 'return number of columns' do
      expect(subject.cols).to eq 9
    end

    it 'return number of rows' do
      expect(subject.rows).to eq 9
    end

    it 'should have method each' do
      i = 0
      subject.each do |e|
        expect(e.to_i).to eq game[i].to_i
        i += 1
      end
    end

    it 'should have method each returing Enumerator when block is not passed' do
      expect(subject.each.class).to eq Enumerator
    end

    it 'checks wheter grid has valid data' do
      expect(subject.valid?).to eq true
    end

    it 'has method for getting first element' do
      expect(subject.first.value).to eq 4
    end

    it 'has method for getting last element' do
      expect(subject.last.value).to eq 9
    end

    it 'has block iterator' do
      block1 = subject.block(0).map(&:to_i)
      expect(block1).to eq [4, 0, 0, 0, 3, 0, 0, 0, 0]
      block2 = subject.block(1).map(&:to_i)
      expect(block2).to eq [0, 0, 0, 0, 0, 0, 7, 0, 0]
      block8 = subject.block(7).map(&:to_i)
      expect(block8).to eq [6, 0, 3, 2, 0, 0, 0, 0, 0]
      block9 = subject.block(8).map(&:to_i)
      expect(block9).to eq [0, 7, 0, 0, 0, 0, 0, 0, 9]
    end
  end

  describe 'setting values' do
    let(:game) do
      '4.....8.5.3..........7......2.....6.....8.' \
      '4......1.......6.3.7.5..2.....1.4......'
    end

    it 'has loaded game' do
      expect(subject.missing).to eq 64
    end

    it 'can replace row' do
      row = [7, 0, 0, 0, 0, 0, 0, 0, 8]
      subject[0] = row
      expect(subject[0]).to eq row
    end

    it 'can set element at specific position' do
      subject[0, 0] = 5
      expect(subject[0, 0].to_i).to eq 5

      subject[0, 3] = 9
      expect(subject.value(0, 3)).to eq 9
    end
  end

  describe 'sub-block access' do
    #  9  0  0 | 3  0  5 | 0  0  1
    it 'yields row elements ' do
      res = []
      subject.row_elems(1) do |e|
        res << e.to_i
      end
      expect(res).to eq([9, 0, 0, 3, 0, 5, 0, 0, 1])
    end

    it 'yields column elements ' do
      res = []
      subject.col_elems(5) do |e|
        res << e.to_i
      end
      expect(res).to eq([0, 5, 6, 2, 0, 8, 9, 3, 0])
    end

    it 'yields middle block (sudoku square) elements ' do
      res = []
      # middle block
      subject.block_elems(3, 3) do |e|
        res << e.to_i
      end
      expect(res).to eq([1, 0, 2, 0, 0, 0, 7, 0, 8])
    end

    it 'yields first block (sudoku square) elements ' do
      res = []
      # first block
      subject.block_elems(1, 1) do |e|
        res << e.to_i
      end
      expect(res).to eq([0, 0, 3, 9, 0, 0, 0, 0, 1])
    end

    it 'yields last block (sudoku square) elements ' do
      res = []
      # first block
      subject.block_elems(8, 8) do |e|
        res << e.to_i
      end
      expect(res).to eq([5, 0, 0, 0, 0, 9, 3, 0, 0])
    end
  end

  describe 'validates grid' do
    let(:game) do
      '4.....8.5.3..........7......2.....6.....' \
      '8.4......1.......6.3.7.5..2.....1.4......'
    end

    it 'validates current state of grid' do
      expect(subject.valid?).to eq true
    end

    it 'duplicate in row' do
      input = '44....8.5.3..........7......2.....' \
      '6.....8.4......1.......6.3.7.5..2.....1.4......'
      g = StringParser.load(input)
      expect(g.valid?).to eq false
    end

    it 'duplicate in column' do
      input = '4.....8.543..........7......2.....6' \
      '.....8.4......1.......6.3.7.5..2.....1.4......'
      g = StringParser.load(input)
      expect(g.valid?).to eq false
    end

    it 'duplicate in block' do
      input = '4.....8.5.4..........7......2.....6' \
      '.....8.4......1.......6.3.7.5..2.....1.4......'
      g = StringParser.load(input)
      expect(g.valid?).to eq false
    end
  end

  it 'has solution method' do
    expect(subject.respond_to?(:solution)).to eq true
    expect(subject.solution).to eq '00302060090030500100180640000810290070000' \
                                   '0008006708200002609500800203009005010300'
  end

  describe 'map method' do
    it 'has map method' do
      expect(grid.respond_to?(:map)).to eq true
      expect(grid.map(&:to_i)).to eq [
        0, 0, 3, 0, 2, 0, 6, 0, 0, 9, 0, 0, 3, 0, 5, 0, 0, 1, 0, 0, 1, 8, 0, 6,
        4, 0, 0, 0, 0, 8, 1, 0, 2, 9, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 8, 0, 0, 6,
        7, 0, 8, 2, 0, 0, 0, 0, 2, 6, 0, 9, 5, 0, 0, 8, 0, 0, 2, 0, 3, 0, 0, 9,
        0, 0, 5, 0, 1, 0, 3, 0, 0
      ]
    end

    it 'sums over sudoku' do
      expect(grid.reduce(0) { |a, e| a + e.to_i }).to eq 159
    end
  end
end

#!/usr/bin/env ruby
$LOAD_PATH << './'
require_relative 'spec_helper'
require 'sudoku'

describe Sudoku do
  subject { Sudoku.new(game) }
  let(:game) do
    '0060028000501300204025009019000705400208' \
    '01090087050002208009305040018070003600100'
  end

  it 'should know when game is solved' do
    expect(subject.solved?).to eq false
  end

  it 'should solve' do
    expect { subject.solve }.to change {
      subject.solved?
    }.from(false).to(true)
    expect(subject.solution).to eq(
      '1364928578591374264725869319612735485248' \
      '61793387954612218749365645318279793625184'
    )
  end

  describe 'simple' do
    let(:game) do
      '0107800000906000450025001000000005678000' \
      '20003136000000007009400620007050000051030'
    end

    it 'should solve' do
      expect { subject.solve }.to change {
        subject.solved?
      }.from(false).to(true)
      expect(subject.solution).to eq(
        '513784692798612345462593178249138567875' \
        '426913136975284357869421621347859984251736'
      )
    end
  end

  describe 'simple 2' do
    let(:game) do
      '0030206009003050010018064000081029007000' \
      '00008006708200002609500800203009005010300'
    end

    it 'should solve' do
      expect { subject.solve }.to change {
        subject.solved?
      }.from(false).to(true)
      expect(subject.solution).to eq(
        '48392165796734582125187649354813297672' \
        '9564138136798245372689514814253769695417382'
      )
    end
  end

  describe 'incomplete/invalid games' do
    let(:game) { '' }
    it 'checks whether grid has valid data' do
      expect { subject.valid? }.to raise_error RuntimeError
    end
  end
end

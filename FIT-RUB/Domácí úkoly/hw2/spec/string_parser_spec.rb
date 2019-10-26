#!/usr/bin/env ruby
$LOAD_PATH << './'
require 'spec_helper'
require_relative 'spec_helper'
require 'grid'

describe StringParser do
  subject { StringParser }

  it 'checks if argument is supported' do
    expect(subject.supports?('')).to eq false
    expect(subject.supports?(nil)).to eq false
    expect(subject.supports?(123)).to eq false
    expect(subject.supports?('a' * 81)).to eq false
  end

  it 'should recognize correct input' do
    inp = '0030206009003050010018064000081029007000' \
          '00008006708200002609500800203009005010300'
    expect(subject.supports?(inp)).to eq true
  end

  it 'should recognize correct input' do
    inp = '4.....8.5.3..........7......2.....6.....8' \
          '.4......1.......6.3.7.5..2.....1.4......'
    expect(subject.supports?(inp)).to eq true
  end

  it 'should return Grid object' do
    inp = '4.....8.5.3..........7......2.....6.....8' \
          '.4......1.......6.3.7.5..2.....1.4......'
    expect(subject.load(inp).class).to eq Grid
  end
end

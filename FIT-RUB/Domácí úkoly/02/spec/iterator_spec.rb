#!/usr/bin/env ruby
require 'rspec'
require_relative 'spec_helper'
require_relative '../01_iterators'

describe 'Odd iterator' do

  context 'yields odd elements' do
    subject(:res) { odd_elements([1, 2, 3, 4, 5, 6]) }
    it { is_expected.to be_an_instance_of Array }
    it { is_expected.not_to be_empty }
    it { expect(res.size).to be 3 }
    it { is_expected.to include(2) }
    it { is_expected.to include(4) }
    it { is_expected.to include(6) }
  end

  context 'different numbers' do
    subject(:ary) { odd_elements([0, 1, 1, 9, 2, 3]) }
    it { is_expected.to be_an_instance_of Array }
    it { is_expected.not_to be_empty }
    it { expect(ary.size).to be 3 }
    it { expect(ary[0]).to be 1 }
    it { expect(ary[1]).to be 9 }
    it { expect(ary[2]).to be 3 }
  end

  context 'should work for float' do
    subject(:res) { odd_elements([15, 4.5, 3.14, 2.7181, 2, 0.123]) }
    it { is_expected.to be_an_instance_of Array }
    it { expect(res.size).to eq 3 }
    it { expect(res[0]).to eq 4.5 }
    it { expect(res[1]).to eq 2.7181 }
    it { expect(res[2]).to eq 0.123 }
  end

  context 'should yield' do
    subject(:res) { odd_elements([1, 2, 3, 4, 5, 6]) { |x| x**2 } }
    it { is_expected.to be_an_instance_of Array }
    it { expect(res.size).to be 3 }
    it { expect(res[0]).to be 4 }
    it { expect(res[1]).to be 16 }
    it { expect(res[2]).to be 36 }
  end

  context 'should return empty array' do
    subject(:res){ odd_elements([1]) { |x| puts x } }
    it { is_expected.to be_an_instance_of Array }
    it { expect(res.size).to be 0 }
    it { is_expected.to be_empty }
  end

  context 'should not modify original array' do
    ary = [2, 2, 2, 5, 5, 8]
    ary2 = ary.dup
    subject(:res) { odd_elements(ary) }
    it { expect(res[0]).to be 2 }
    it { expect(res[1]).to be 5 }
    it { expect(res[2]).to be 8 }
    it { expect(ary).to eq ary2 }
  end

  context 'should not modify original array when passing a block' do
    ary = [2, 2, 2, 5, 5, 8]
    ary2 = ary.dup
    subject(:res) { odd_elements(ary) { |x| x + 1 } }
    it { expect(ary).to eq ary2 }
  end

  context 'should yield only odd elements' do
    ary = [false, true]
    odd_elements(ary) do |x|
      it { expect(x).to be true }
    end
  end

end

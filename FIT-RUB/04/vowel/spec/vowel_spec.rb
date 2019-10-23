#!/usr/bin/env ruby
require_relative '../vowel_finder'
require_relative 'spec_helper'

describe VowelFinder do

  subject(:vf) { described_class.new(data) }
  let(:data) { 'the quick brown fox jumped' }

  it 'find vowels' do
    expect(vf.map.to_a).to eq(%w(e u i o o u e))
  end

  describe '#sum' do
    let(:data) { 'The quick brown fox jumps over the lazy dog' }
    it 'has method #sum' do
      expect(vf.methods.include?(:sum)).to be true
    end

    it 'performs a sum' do
      expect(vf.sum).to eq 'euioouoeeayo'
    end
  end

  context 'capital letters' do
    let(:data) do
      'A CRARY BIG BUY JUMPS OVER A HORIZONTALLY
        SIGNIFICANT LADY'
    end

    it 'downcase capital letters' do
      expect(vf.sum).to eq 'aayiuyuoeaoioayiiiaay'
    end
  end

  context 'Summable module' do
    let(:ary) { [1, 2, 3] }

    it 'exists' do
      expect(VowelFinder.const_defined?(:Summable)).to be true
    end

    it 'include Summable module in Array class' do
      expect(ary.methods.include?(:sum)).to be true
    end

    let(:ary) { [1, 2, 3, 4] }
    it 'handles sum over Array class' do
      expect(ary.sum).to eq 10
    end
  end

  context 'Enumerator' do
    let(:data) { 'hello' }

    it 'returns an Enumerator' do
      expect(vf.each.instance_of?(Enumerator)).to be true
    end

    it 'handles reduce' do
      expect(vf.reduce(:+)).to eq 'eo'
    end

    it 'handles map' do
      expect(vf.map(&:succ)).to eq %w(f p)
    end

    it 'handles max' do
      expect(vf.max).to eq 'o'
    end

    it 'handles min' do
      expect(vf.min).to eq 'e'
    end
  end
end

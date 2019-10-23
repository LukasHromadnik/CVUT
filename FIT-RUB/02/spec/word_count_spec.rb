#!/usr/bin/env ruby
require 'rspec'
require_relative 'spec_helper'
require_relative '../03_word_count'

describe 'word count' do
  context 'Hash with word counts in first file' do
    subject(:res) { word_count("#{File.dirname(__FILE__)}/words/01.txt") }
    it { is_expected.to be_an_instance_of Hash }
    it { expect(res.size).to be 18 }
    it { expect(res.key?(:test)).to be true }
    it { expect(res[:test]).to be 3 }
    it { expect(res.key?(:simple)).to be true }
    it { expect(res[:simple]).to be 2 }
  end

  context 'Hash with word counts in second file' do
    subject(:res) { word_count("#{File.dirname(__FILE__)}/words/02.txt") }
    it { is_expected.to be_an_instance_of Hash }
    it { expect(res.key?(:there)).to be true }
    it { expect(res[:there]).to be 2 }
    it { expect(res[:the]).to be 4 }
    it { expect(res[:oh]).to be 3 }
  end

  context 'Hamlet monologue' do
    subject(:res) { word_count("#{File.dirname(__FILE__)}/words/03.txt") }
    it { is_expected.to be_an_instance_of Hash }
    it { expect(res.key?(:to)).to be true }
    it { expect(res.key?(:be)).to be true }
    it { expect(res.key?(:or)).to be true }
    it { expect(res.key?(:not)).to be true }
    it { expect(res[:to]).to be 15 }
    it { expect(res[:be]).to be 4 }
  end
end

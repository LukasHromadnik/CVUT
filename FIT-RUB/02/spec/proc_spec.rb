#!/usr/bin/env ruby
require 'rspec'
require_relative 'spec_helper'
require_relative '../02_proc'

describe 'Uppercasing procedure' do
  context 'returns uppercased string if starts with \'a\'' do
    subject(:res) { bring_up.call('alphabet') }
    it { is_expected.to be_an_instance_of String }
    it { is_expected.not_to be_empty }
    it { is_expected.to eq 'ALPHABET' }
  end

  context 'returns uppercased string if starts with \'A\'' do
    subject(:res) { bring_up.call('Alabama') }
    it { is_expected.to be_an_instance_of String }
    it { is_expected.not_to be_empty }
    it { is_expected.to eq 'ALABAMA' }
  end

  context 'returns original otherwise (other)' do
    text = 'beta'
    subject(:res) { bring_up.call(text) }
    it { is_expected.to be_an_instance_of String }
    it { is_expected.not_to be_empty }
    it { is_expected.to be text }
  end

  context 'returns original otherwise (number)' do
    text = '123'
    subject(:res) { bring_up.call(text) }
    it { is_expected.to be_an_instance_of String }
    it { is_expected.not_to be_empty }
    it { is_expected.to be text }
  end

  context 'returns original value for Integer' do
    number = 42
    subject(:res) { bring_up.call(number) }
    it { is_expected.to be_an_instance_of Fixnum }
    it { is_expected.to be number }
  end

  context 'does not modify original value' do
    text = 'Alabama'
    subject(:res) { bring_up.call(text) }
    it { is_expected.to be_an_instance_of String }
    it { is_expected.not_to be_empty }
    it { is_expected.to eq 'ALABAMA' }
    it 'should stay the same' do
      expect { text == 'Alabama' }
    end
  end
end

describe 'Doubling procedure' do
  context 'returns double value if number' do
    subject(:res) { make_double.call(10) }
    it { is_expected.to be_an_instance_of String }
    it { is_expected.not_to be_empty }
    it { is_expected.to eq '20' }
  end

  context 'returns double value if string' do
    subject(:res) { make_double.call('20') }
    it { is_expected.to be_an_instance_of String }
    it { is_expected.not_to be_empty }
    it { is_expected.to eq '40' }
  end

  context 'returns original otherwise (float)' do
    number = 3.1415926
    subject(:res) { make_double.call(number) }
    it { is_expected.to be_an_instance_of Float }
    it { is_expected.to be number }
  end

  context 'returns original otherwise (string)' do
    text = 'It is something!'
    subject(:res) { make_double.call(text) }
    it { is_expected.to be_an_instance_of String }
    it { is_expected.to be text }
  end

  context 'does not modify original value' do
    text = '120'
    subject(:res) { make_double.call(text) }
    it { is_expected.to be_an_instance_of String }
    it { is_expected.not_to be_empty }
    it { is_expected.to eq '240' }
    it 'should stay the same' do
      expect { text == '120' }
    end
  end
end

describe 'Wage increase' do
  context 'increase wages in correct contract' do
    subject(:res) { increase_wage('You get 100 CZK') }
    it { is_expected.to be_an_instance_of String }
    it { is_expected.not_to be_empty }
    it { is_expected.to eq 'You get 200 CZK' }
  end

  context 'increase all numbers' do
    subject(:res) { increase_wage('You get 200 CZK per 2 hours') }
    it { is_expected.to be_an_instance_of String }
    it { is_expected.not_to be_empty }
    it { is_expected.to eq 'You get 400 CZK per 4 hours' }
  end

  context 'do not change nonnumerics' do
    subject(:res) { increase_wage('You get hundred CZK') }
    it { is_expected.to be_an_instance_of String }
    it { is_expected.not_to be_empty }
    it { is_expected.to eq 'You get hundred CZK' }
  end
end

describe 'Array processing' do
  context 'process an array of strings' do
    subject(:res) { process_text_array %w(Alex has 20 CZK) }
    it { is_expected.to be_an_instance_of Array }
    it { is_expected.not_to be_empty }
    it { is_expected.to eq %w(ALEX has 40 CZK) }
  end

  context 'process mixed with numbers' do
    subject(:res) { process_text_array(['Your', 'Alice', 'has', 10, 'CZK']) }
    it { is_expected.to be_an_instance_of Array }
    it { is_expected.not_to be_empty }
    it { is_expected.to eq %w(Your ALICE has 20 CZK) }
  end
end

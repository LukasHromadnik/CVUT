#!/usr/bin/env ruby
require_relative '../my_struct'
require_relative 'spec_helper'
require 'ostruct'

describe MyStruct do

  subject(:struct) { described_class.new(data) }
  let(:data) { {} }

  context 'basic behaviour' do
    it 'does not inherit from OpenStruct' do
      expect(struct.is_a?(OpenStruct)).to eq false
      expect(struct.is_a?(Hash)).to eq false
    end

    it 'has method size' do
      expect(struct.respond_to?(:size)).to eq true
      expect(struct.size).to eq 0
    end
  end

  context 'struct initialization' do
    let(:data) { { foo: 'bar', one: 1 } }
    it 'is initialized with given Hash' do
      expect(struct.foo).to eq 'bar'
      expect(struct.one).to eq 1
    end

    it 'returns correct size' do
      expect(struct.size).to eq 2
    end

    it 'has getter []' do
      expect(struct[:foo]).to eq 'bar'
      expect(struct[:one]).to eq 1
    end

    it 'has setter []=' do
      struct[:ruby] = 'is great'
      expect(struct[:ruby]).to eq 'is great'
    end
  end

  context 'each_pair method' do
    let(:data) { { firstname: 'John', surname: 'Doe', age: 50, size: 20 } }
    it 'has each_pair method' do
      expect(struct.respond_to?(:each_pair)).to eq true
    end

    it 'returns all pairs' do
      expect(struct.each_pair.class).to eq Enumerator
      expect(struct.each_pair.first).to eq [:firstname, 'John']
      expect(struct.each_pair.to_a).to eq [
        [:firstname, 'John'], [:surname, 'Doe'], [:age, 50], [:size, 20]
      ]
    end

    it 'returns correct size' do
      expect(struct.size).to eq 4
    end

    it 'getter works' do
      expect(struct[:size]).to eq 20
    end

    it 'setter works' do
      struct[:size] = 55
      expect(struct[:size]).to eq 55
    end
  end

  context 'iterators' do
    let(:data) { { one: 5, two: 10, three: 15, four: 20 } }

    it 'has method each value' do
      expect(struct.each_value.class).to eq Enumerator
      expect(struct.each_value.max).to eq 20
    end

    it 'has method each key' do
      expect(struct.each_key.class).to eq Enumerator
      expect(struct.each_key.max).to eq :two
    end

    it 'works!' do
      expect(struct.each_value.reduce(:+)).to eq 50
    end
  end

end

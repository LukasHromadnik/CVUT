#!/bin/ruby
# -*- encoding : utf-8 -*-
require_relative 'spec_helper'
require_relative '../triangle'

describe 'Triangle' do

  context 'existence' do
    it 'is not valid' do
      expect { triangle(1, 1, 3) }.to raise_error TriangleError
    end
  end

  context 'proportion' do
    it 'should be equilateral' do
      expect(triangle(1, 1, 1)).to eq :equilateral
    end

    it 'should be isosceles' do
      expect(triangle(1, 2, 2)).to eq :isosceles
    end

    it 'should be scalene' do
      expect(triangle(2, 5, 6)).to eq :scalene
    end
  end

end

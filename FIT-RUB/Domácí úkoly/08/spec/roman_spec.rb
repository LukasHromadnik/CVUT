require 'rspec'
require_relative 'spec_helper'
require_relative '../roman.rb'

describe Roman do

  context 'small numbers' do
    it 'to_s converts arabic numbers to roman' do
      [
        [1, 'i'],
        [2, 'ii'],
        [3, 'iii'],
        [4, 'iv'],
        [5, 'v'],
        [6, 'vi'],
        [7, 'vii'],
        [8, 'viii'],
        [9, 'ix'],
        [10, 'x'],
        [11, 'xi'],
        [12, 'xii'],
        [13, 'xiii'],
        [14, 'xiv'],
        [158, 'clviii'],
        [3939, 'mmmcmxxxix']
      ].each do |key, value|
        expect(Roman.new(key).to_s.downcase).to eq value
      end
    end
  end

  context 'act as an integer' do
    subject(:roman) { Roman.new(1) }

    it 'responds as an integer' do
      expect(roman.respond_to?(:to_int)).to be true
      expect(roman.to_int).to eq 1
      expect(roman.respond_to?(:to_i)).to be true
      expect(roman.to_i).to eq 1
    end
  end

  context 'basic numerical operations' do
    subject(:res) { Roman.new(expr) }

    context 'adding 2 + 3' do
      let(:expr) { Roman.new(2) + Roman.new(3) }

      it 'returns correct result' do
        expect(res).to eq 5
        expect(res.kind_of? Roman).to be true
      end
    end

    context 'adding 1 + 1' do
      let(:expr) { Roman.new(1) + Roman.new(1) }
      it 'returns correct result' do
        expect(res).to eq 2
        expect(res.kind_of? Roman).to be true
      end
    end

    context 'coerce methods' do
      let(:expr) { 2 + Roman.new(3) }
      it 'returns correct result' do
        expect(res).to eq 5
        expect(res.kind_of? Roman).to be true
      end
    end

    context 'coerce method - in any order' do
      let(:expr) { Roman.new(3) + 3 }
      it 'returns correct result' do
        expect(res).to eq 6
        expect(res.kind_of? Roman).to be true
      end
    end

    context 'multiplying roman * roman' do
      let(:expr) { Roman.new(3) * Roman.new(3) }
      it 'returns correct result' do
        expect(res).to eq 9
        expect(res.kind_of? Roman).to be true
      end
    end

    context 'multiplying int * roman' do
      let(:expr) { 2 * Roman.new(3) }
      it 'returns correct result' do
        expect(res).to eq 6
        expect(res.kind_of? Roman).to be true
      end
    end

    context 'multiplying roman * int' do
      let(:expr) { Roman.new(6) * 7 }
      it 'returns 42' do
        expect(res).to eq 42
        expect(res.kind_of? Roman).to be true
      end
    end

    context 'substraction roman - roman' do
      let(:expr) { Roman.new(5) - Roman.new(3) }
      it 'returns correct result' do
        expect(res).to eq 2
        expect(res.kind_of? Roman).to be true
      end
    end

    context 'substraction roman - int' do
      let(:expr) { Roman.new(4) - 2 }
      it 'returns correct result' do
        expect(res).to eq 2
        expect(res.kind_of? Roman).to be true
      end
    end

    context 'substraction int - roman' do
      let(:expr) { 44 - Roman.new(2) }
      it 'returns 42' do
        expect(res).to eq 42
        expect(res.kind_of? Roman).to be true
      end
    end

    context 'division roman / roman' do
      let(:expr) {  Roman.new(12) / Roman.new(4) }
      it 'returns correct result' do
        expect(res).to eq 3
        expect(res.kind_of? Roman).to be true
      end
    end

    context 'division roman / int' do
      let(:expr) {  Roman.new(12) / 6 }
      it 'returns correct result' do
        expect(res).to eq 2
        expect(res.kind_of? Roman).to be true
      end
    end

    context 'division roman / float' do
      let(:expr) {  Roman.new(12) / 4.0 }
      it 'returns correct result' do
        expect(res).to eq 3
        expect(res.kind_of? Roman).to be true
      end
    end

    context 'division float / roman' do
      let(:expr) {  20 / Roman.new(4) }
      it 'returns correct result' do
        expect(res).to eq 5
        expect(res.kind_of? Roman).to be true
      end
    end
  end

  context 'comparable' do
    it 'supports basic comparing operators' do
      expect(Roman.new(2) > Roman.new(1)).to be true
      expect(Roman.new(1) < Roman.new(3)).to be true
      expect(Roman.new(2) == Roman.new(2)).to be true
      expect(Roman.new(2) != Roman.new(3)).to be true
      expect(Roman.new(2) <= Roman.new(3)).to be true
      expect(Roman.new(5) >= Roman.new(3)).to be true
      expect(Roman.new(5) >= Roman.new(5)).to be true
      expect(Roman.new(5).between?(Roman.new(2), Roman.new(6))).to be true
    end

    it 'supports spaceship operator' do
      expect(Roman.new(5) <=> Roman.new(3)).to eq 1
      expect(Roman.new(5) <=> Roman.new(5)).to eq 0
    end

    it 'compare types and values' do
      expect(Roman.new(2).eql?(Roman.new(2))).to be true
      expect(Roman.new(2).eql?(Roman.new(3))).to be false
      expect(Roman.new(2).eql?(2)).to be false
    end

    it 'to float' do
      expect(Roman.new(1)).to eq 1.0
      expect(Roman.new(1)).not_to eq 1.2
    end

    it 'has defined equal? methods' do
      r = Roman.new(5)
      expect(r.equal?(r)).to be true
      expect(r.equal?(Roman.new(5))).to be false
    end
  end
end

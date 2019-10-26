require 'rspec'
require_relative 'spec_helper'
require_relative '../my_array'

describe 'Our version of' do
  shared_examples 'MyArray' do

    subject(:array) { described_class.new(data) }
    let(:data) { [1, 2, 3] }


    describe '#initialize' do
      it 'copies data from a given array' do
        ary = described_class.new([1, 2, 3])
        expect(ary.size).to eq 3
      end
    end

    describe '#size' do
      it 'reflects a number of elements' do
        expect(array.size).to eql 3
      end

      context 'for an empty array' do
        subject { described_class.new.size }

        it { is_expected.to eq 0 }
      end
    end

    describe '#[]' do
      let(:data) { [2, 5, 3] }

      specify { expect(data[0]).to eq 2 }
      specify { expect(data[1]).to eq 5 }
      specify { expect(data[2]).to eq 3 }
    end

    describe '#each' do
      it 'iterates over data' do
        d = []
        array.each do |i|
          d << i
        end
        expect(d).to eq data
      end

      context 'without a given block' do
        subject(:each) { array.each }

        it 'returns an Enumerator' do
          expect(each.class).to eq Enumerator
          expect(each.to_a).to eq data
        end
      end
    end

    describe 'has reverse methods' do
      let(:data) { [2, 5, 3] }
      let(:revdata) { [3, 5, 2] }

      describe '#reverse_each' do
        it 'enumerates in reverse' do
          expect(array.reverse_each.class).to eq Enumerator
          expect(array.reverse_each.to_a).to eq(revdata)
        end
      end

      describe '#reverse' do
        it 'returns an Array' do
          res = array.reverse
          expect(res.class).to eq Array
          expect(res.to_a).to eq revdata
        end
      end

      describe '#reverse!' do
        it 'modifies itself' do
          array.reverse!
          expect(array.each.to_a).to eq([3, 5, 2])
        end
      end
    end

    describe '#pop' do
      let(:data) { [2, 5, 3] }

      it 'returns the last element' do
        expect(array.pop).to eq 3
      end

      it 'removes the last element' do
        expect { array.pop }.to change { array.size }.from(3).to(2)
      end
    end

    describe '#select' do
      let(:data) { [1, 2, 3, 4, 5, 6] }

      it 'selects elements matching a given block' do
        res = array.select(&:even?)
        expect(res).to eq [2, 4, 6]
      end
    end

    describe '#map' do
      context 'without a given block' do
        it 'returns an Enumerator' do
          res = array.map
          expect(res.class).to eq Enumerator
        end
      end

      context 'with strings' do
        let(:data) { %w(a b c d) }

        it 'iterates over elements and collects block results' do
          res = array.map { |x| x + '!' }
          expect(res).to eq %w( a! b! c! d!)
        end
      end

      context 'with numbers' do
        let(:data) { [1, 2, 3, 4] }

        it 'does not modify the original array' do
          res = array.map { |x| x**2 }
          expect(res).to eq([1, 4, 9, 16])
          expect(array.each.to_a).to eq([1, 2, 3, 4])
        end
      end
    end

    describe '#clear' do
      let(:data) { %w(a b c d) }

      it 'removes all elements' do
        expect { array.clear }.to change { array.size }.from(4).to(0)
      end
    end

    describe '#include?' do
      let(:data) { [2, 5, 15, 23] }

      it 'returns true if an element is in the array' do
        res = array.include?(5)
        expect(res).to eq true
      end

      it 'returns false if an element is not in the array' do
        res = array.include?('duck')
        expect(res).to eq false
      end
    end

    describe '#max' do
      let(:data) { (1..50).to_a }

      it 'returns the maximum value' do
        expect(array.max).to eq 50
      end

      context 'with a given block' do
        let(:data) { %w(dog horse albatross duck) }

        it 'compares by a given block' do
          res = array.max { |a, b| a.length <=> b.length }
          expect(res).to eq 'albatross'
        end
      end
    end

  end

  describe MyArray do
    it_behaves_like 'MyArray'
  end

  describe Array do
    it_behaves_like 'MyArray'
  end
end

require File.expand_path(File.dirname(__FILE__) + '/spec_helper')

describe 'MlDistance' do

  context 'Euclidean Distance' do

    subject { [5, 5] }

    it "has 'euclidean' method" do
      expect(subject.respond_to?(:euclidean)).to be true
    end

    it 'distance to self is equal to 0.0' do
      expect(subject.euclidean(subject)).to eq 0.0
    end

    it 'should be 4.0' do
      expect([5].euclidean([1])).to eq 4.0
    end

    it 'two dimensions vector' do
      expect([1, 6].euclidean([4, 2])).to eq 5.0
    end

    it 'raises an error in case of different vector sizes' do
      expect { [1].euclidean([1, 2]) }.to raise_error(ArgumentError)
    end
  end

end

require File.expand_path(File.dirname(__FILE__) + '/spec_helper')

describe 'MlDistance' do

  DELTA = 1e-8

  context 'Dot Product' do
    it 'returns 50' do
      expect([5, 5].dot_product([5, 5])).to eq 50.0
    end
  end

  context 'cosine similarity' do

    it 'should compute cosine similarity' do
      x = [1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1]
      y = [0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1]
      expect(x.cosine(y)).to be_within(DELTA).of(0.33333333333333337)
    end

    it 'should compute cosine similarity' do
      x = [1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1]
      y = [1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 1]
      expect(x.cosine(y)).to be_within(DELTA).of(0.5163977794943222)
    end
  end
end

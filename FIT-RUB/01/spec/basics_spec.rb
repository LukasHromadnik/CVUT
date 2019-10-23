require 'rspec'
require_relative 'spec_helper'
require_relative '../basics.rb'

describe 'basics' do
  it 'knows the meaning of the world' do
    expect(meaning_of_the_world).to eq 42
  end

  it 'returns 3 elements in an array' do
    expect(array_with_three_elements.class).to eq Array
    expect(array_with_three_elements.size).to eq 3
  end

  it 'returns a hash' do
    subj = hash_with_one_key
    expect(subj.class).to eq Hash
    expect(subj.size).to eq 1
  end

  context 'convert things into other things' do
    it 'reverses text in the argument' do
      subj = reverse_arg('dog')
      expect(subj).to eq 'god'
    end

    it 'reverses array in the argument' do
      subj = reverse_arg([5, 8, 4])
      expect(subj).to eq [4, 8, 5]
    end

    it 'shouldn\'t modify the argument' do
      ary = [5, 8, 4]
      arg = ary.dup
      reverse_arg(arg)
      expect(ary).to eq arg
    end
  end

  context 'upcase string' do
    it 'should convert to upper case' do
      subj = capitalize_arg('hello world')
      expect(subj).to eq 'HELLO WORLD'
    end

    it 'shouldn\'t modify argument' do
      str = 'hello world'
      arg = str.dup
      capitalize_arg(arg)
      expect(str).to eq arg
    end
  end

  it 'should return 3rd element of string' do
    subj = third_elem('ahoj')
    expect(subj).to eq 'o'
  end

  it 'should return 3rd element of an array' do
    subj = third_elem([1, 2, 3, 4])
    expect(subj).to eq 3
  end

  it 'should return nil' do
    subj = third_elem('cz')
    expect(subj).to eq nil
  end
end

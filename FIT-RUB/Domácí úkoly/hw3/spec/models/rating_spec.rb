require 'spec_helper'

describe Rating do

  before(:each) do
    # build doesn't save object, #create saves object to a database
    @rating = FactoryGirl.build(:rating)
  end

  it 'has a valid factory' do
    expect(@rating).to be_valid
  end

  it 'has numeric rating' do
    @rating.rating = 3
    expect(@rating).to be_valid
  end

  it 'has between 1 and 5' do
    @rating.rating = 0
    expect(@rating).not_to be_valid
  end

  it 'has rating' do
    @rating.rating = nil
    expect(@rating).not_to be_valid
  end

  it "can't be 6" do
    @rating.rating = 6
    expect(@rating).not_to be_valid
  end

  it 'has between 1 and 5' do
    @rating.rating = 'dve hvezdy'
    expect(@rating).not_to be_valid
  end
end

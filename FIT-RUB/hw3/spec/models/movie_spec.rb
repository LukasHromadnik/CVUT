require 'spec_helper'

describe Movie do

  before(:each) do
    # build doesn't save object, #create saves object to a database
    @movie = FactoryGirl.build(:movie)
  end

  it 'has a valid factory' do
    expect(@movie).to be_valid
  end

  it 'is invalid without a title ' do
    @movie.title = nil
    expect(@movie).not_to be_valid
  end

  it "has a method 'genre?' which accepts string as a param" do
    expect(@movie.respond_to?('genre?')).to eq true
  end
end

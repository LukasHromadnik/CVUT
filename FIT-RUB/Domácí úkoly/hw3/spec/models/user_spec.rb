require 'spec_helper'

describe User do

  before(:each) do
    # build doesn't save object, #create saves object to a database
    @user = FactoryGirl.build(:user)
  end

  it 'has a valid factory' do
    expect(@user).to be_valid
  end

  it 'is invalid without an age ' do
    @user.age = nil
    expect(@user).not_to be_valid
  end

  it 'age should be a number' do
    @user.age = 'triadvacet'
    expect(@user).not_to be_valid
  end

  it 'pi is not a valid age' do
    @user.age = 3.14159265359
    expect(@user).not_to be_valid
  end

  it 'has age greated than 0' do
    @user.age = -325
    expect(@user).not_to be_valid
  end

  it "has gender either 'M' or 'F'" do
    @user.gender = 'M'
    expect(@user).to be_valid

    @user.gender = 'F'
    expect(@user).to be_valid
  end

  it "gender can't have multiple letters" do
    @user.gender = 'male'
    expect(@user).not_to be_valid

    @user.gender = 'woman'
    expect(@user).not_to be_valid
  end

  context 'k-nn' do
    it 'find k-nearest neighours (cosine)' do
      user = User.find(1)
      res = user.knn(5, :cosine)
      expect(res).to eq [823, 514, 864, 592, 521]
    end

    it 'find k-nearest neighours (euclidean)' do
      user = User.find(1)
      res = user.knn(10, :euclidean)
      #res.each do |e|
      #  u = User.find(e)
      #  a, b = user.common(user,u)
      #  puts "a: " << a.to_s
      #  puts "b #{e}: " << b.to_s
      #  puts "dist = #{Distance.euclidean(a, b)}"
      #end
      expect(res).to eq [182, 723, 248, 51, 124, 202, 162, 700, 55, 209]
    end
  end
end

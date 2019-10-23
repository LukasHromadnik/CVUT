require 'spec_helper'

describe Movies::UsersAPI do

  it 'shows list of users, default limit is 20' do
    with_api(Movies::App) do
      get_request(path: '/v1/users') do |c|
        resp = JSON.parse(c.response)
        expect(resp.size).to eq 20
      end
    end
  end

  it 'list of users supports limit' do
    with_api(Movies::App) do
      get_request(path: '/v1/users?limit=10') do |c|
        resp = JSON.parse(c.response)
        expect(resp.size).to eq 10
      end
    end
  end

  it 'users supports limit and offset - occupation after 20. position' do
    with_api(Movies::App) do
      get_request(path: '/v1/users?limit=10&offset=30') do |c|
        resp = JSON.parse(c.response)
        expect(resp.size).to eq 10
        expect(resp.first['age']).to eq 24
        expect(resp.first['gender']).to eq 'M'
      end
    end
  end

  it 'finds user by an id' do
    with_api(Movies::App) do
      get_request(path: '/v1/users/45') do |c|
        resp = JSON.parse(c.response)
        expect(resp['age']).to eq 29
        expect(resp['gender']).to eq 'M'
        expect(resp['zip']).to eq '50233'
      end
    end
  end

  context 'ratings' do
    it 'gets all ratings by given user' do
      with_api(Movies::App) do
        get_request(path: '/v1/users/125/ratings') do |c|
          resp = JSON.parse(c.response)
          expect(resp.size).to eq 90
        end
      end
    end

    it 'gets average rating by given user' do
      with_api(Movies::App) do
        get_request(path: '/v1/users/100/avg_rating') do |c|
          resp = JSON.parse(c.response)
          expect(resp).to include 'avg_rating'
          expect(resp['avg_rating']).to be_within(DELTA).of(3.0294117647058822)
        end
      end
    end
  end

  context 'knn (Euclidean distance)' do
    it 'gets 5 nearest neighbours to given user' do
      with_api(Movies::App) do
        err = proc do |e|
          fail "ERROR: #{e.error}"
        end
        start = Time.now.to_f
        get_request({path: '/v1/users/100/knn'}, err) do |c|
          resp = JSON.parse(c.response)
          expect(resp['result']).to eq [12, 17, 27, 24, 28]
          expect(resp['result'].size).to eq 5
          time = (Time.now.to_f - start)
          expect(time).to be < 6
          puts "time %.2f" % time
        end
      end
    end

    it 'gets 10 nearest neighbours to given user' do
      with_api(Movies::App) do
        err = proc do |e|
          fail "ERROR: #{e.error}"
        end
        start = Time.now.to_f
        get_request({path: '/v1/users/200/knn?k=10'}, err) do |c|
          resp = JSON.parse(c.response)
          expect(resp['result']).to eq [9, 51, 96, 127, 132, 179, 133, 134, 143, 185]
          expect(resp['result'].size).to eq 10
          time = (Time.now.to_f - start)
          expect(time).to be < 6
          puts "time %.2f" % time
        end
      end
    end
  end

  context 'knn (Cosine distance)' do
    it 'gets 5 nearest neighbours to given user' do
      with_api(Movies::App) do
        err = proc do |e|
          fail "ERROR: #{e.error}"
        end
        start = Time.now.to_f
        get_request({path: '/v1/users/41/knn?metric=cosine'}, err) do |c|
          resp = JSON.parse(c.response)
          expect(resp['result']).to eq [148, 738, 686, 748, 508]
          expect(resp['result'].size).to eq 5
          time = (Time.now.to_f - start)
          expect(time).to be < 6
          puts "time %.2f" % time
        end
      end
    end

    it 'gets 10 nearest neighbours to given user' do
      with_api(Movies::App) do
        err = proc do |e|
          fail "ERROR: #{e.error}"
        end
        start = Time.now.to_f
        get_request({path: '/v1/users/139/knn?k=10&metric=cosine'}, err) do |c|
          resp = JSON.parse(c.response)
          expect(resp['result']).to eq [203, 743, 590, 733, 17, 834, 735, 906, 526, 81]
          expect(resp['result'].size).to eq 10
          time = (Time.now.to_f - start)
          expect(time).to be < 6
          puts "time %.2f" % time
        end
      end
    end
  end
end

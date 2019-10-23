require 'spec_helper'

describe Movies::OccupationAPI do

  it 'shows list of occupations, default limit is 20' do
    with_api(Movies::App) do
      get_request(path: '/v1/occupations') do |c|
        resp = JSON.parse(c.response)
        expect(resp.size).to eq 20
      end
    end
  end

  it 'list of occupations supports limit' do
    with_api(Movies::App) do
      get_request(path: '/v1/occupations?limit=10') do |c|
        resp = JSON.parse(c.response)
        expect(resp.size).to eq 10
      end
    end
  end

  it 'occupations supports limit and offset - occupation after 20. position' do
    with_api(Movies::App) do
      get_request(path: '/v1/occupations?limit=10&offset=20') do |c|
        resp = JSON.parse(c.response)
        expect(resp.first['title']).to eq 'writer'
      end
    end
  end

  it 'finds occupation by an id' do
    with_api(Movies::App) do
      get_request(path: '/v1/occupations/21') do |c|
        resp = JSON.parse(c.response)
        expect(resp['title']).to eq 'writer'
      end
    end
  end

  it 'finds all users with given occupation -- artists' do
    with_api(Movies::App) do
      get_request(path: '/v1/occupations/2/users') do |c|
        resp = JSON.parse(c.response)
        expect(resp.size).to be > 20
        resp.each do |o|
          expect(o['occupation_id']).to eq 2
        end
      end
    end
  end

  it 'creates new occupation' do
    with_api(Movies::App) do
      body = { occupation: { title: 'homeless' } }
      req = {
        path: '/v1/occupations/create',
        body: body.to_json,
        head: { 'content-type' => 'application/json' }
      }
      err = proc { fail 'API request failed' }

      # second argument is callback
      post_request(req, err) do |c|
        expect(c.state).to eq :finished
        resp = JSON.parse(c.response)
        expect(resp).to include 'title'
        expect(resp).to include 'id'
        expect(resp['title']).to eq 'homeless'
        expect(c.response_header.status).to be <= 201
      end
    end
  end

  it 'does not accept overriding id' do
    with_api(Movies::App) do
      body = { occupation: { id: 1, title: 'homeless' } }
      req = {
        path: '/v1/occupations/create',
        body: body.to_json,
        head: { 'content-type' => 'application/json' }
      }
      err = proc { fail 'API request failed' }

      # second argument is callback
      post_request(req, err) do |c|
        expect(c.state).to eq :finished
        resp = JSON.parse(c.response)
        expect(resp).to include 'title'
        expect(resp['id'].to_i).not_to eq 1
        expect(resp['title']).to eq 'homeless'
        expect(c.response_header.status).to be <= 201
      end
    end
  end

end

require 'spec_helper'
require 'open-uri'

describe Movies::MoviesAPI do

  DELTA = 1e-8

  it 'shows list of movies, default limit is 20' do
    with_api(Movies::App) do
      get_request(path: '/v1/movies') do |c|
        resp = JSON.parse(c.response)
        expect(resp.size).to eq 20
      end
    end
  end

  it 'list of movies supports limit' do
    with_api(Movies::App) do
      get_request(path: '/v1/movies?limit=10') do |c|
        resp = JSON.parse(c.response)
        expect(resp.size).to eq 10
      end
    end
  end

  it 'lists a movies with all attributes' do
    with_api(Movies::App) do
      get_request(path: '/v1/movies?limit=1') do |c|
        resp = JSON.parse(c.response)
        expect(resp.size).to eq 1
        m = resp.first
        expect(m.include?('release_date')).to eq true
        expect(m.include?('video_release_date')).to eq true
        expect(m.include?('imdb_url')).to eq true
      end
    end
  end

  it 'movies supports limit and offset - movies after 100. position' do
    with_api(Movies::App) do
      get_request(path: '/v1/movies?limit=10&offset=100') do |c|
        resp = JSON.parse(c.response)
        expect(resp.size).to eq 10
        expect(resp.first['title']).to eq 'Heavy Metal (1981)'
      end
    end
  end

  it 'shows list of movies, first one is Toy Story' do
    with_api(Movies::App) do
      get_request(path: '/v1/movies') do |c|
        resp = JSON.parse(c.response)
        expect(resp.first['title']).to eq 'Toy Story (1995)'
      end
    end
  end

  it 'finds movie by id' do
    with_api(Movies::App) do
      get_request(path: '/v1/movies/show/110') do |c|
        resp = JSON.parse(c.response)
        expect(resp['title']).to eq 'Operation Dumbo Drop (1995)'
      end
    end
  end

  it 'finds movie by genre' do
    with_api(Movies::App) do
      get_request(path: '/v1/movies/genre/action') do |c|
        resp = JSON.parse(c.response)
        expect(resp.size).to be > 20
        resp.each do |m|
          expect(Movie.find(m['id']).genre?('action')).to eq true
        end
      end
    end
  end

  context 'search' do
    it 'finds Star Wars' do
      with_api(Movies::App) do
        get_request(path: "/v1/movies/search/#{URI.encode('star wars')}") do |c|
          resp = JSON.parse(c.response)
          expect(resp.first['title']).to eq 'Star Wars (1977)'
        end
      end
    end

    it 'search for all Batman movies' do
      with_api(Movies::App) do
        get_request(path: "/v1/movies/search/#{URI.encode('batman')}") do |c|
          resp = JSON.parse(c.response)
          movies = resp.map { |m| m['title'] }
          expect(movies.include?('Batman Forever (1995)')).to eq true
          expect(movies.include?('Batman Returns (1992)')).to eq true
          expect(movies.include?('Batman & Robin (1997)')).to eq true
          expect(movies.include?('Batman (1989)')).to eq true
        end
      end
    end

    context 'ratings' do
      it 'knows Toy Story rating' do
        with_api(Movies::App) do
          get_request(path: '/v1/movies/rating/1') do |c|
            resp = JSON.parse(c.response)
            expect(resp.first['title']).to eq 'Toy Story (1995)'
            expect(resp.first['rating_avg'].to_f).to be_within(DELTA).of(3.8929503916449086)
            expect(resp.first['movie_id']).to eq 1
          end
        end
      end

      it 'knows The Game rating' do
        with_api(Movies::App) do
          get_request(path: '/v1/movies/rating/333') do |c|
            resp = JSON.parse(c.response)
            expect(resp.first['title']).to eq 'Game, The (1997)'
            expect(resp.first['rating_avg'].to_f).to be_within(DELTA).of(3.5893719806763285)
            expect(resp.first['movie_id']).to eq 333
          end
        end
      end
    end

    it 'top movies' do
      with_api(Movies::App) do
        get_request(path: '/v1/movies/top?limit=8') do |c|
          resp = JSON.parse(c.response)
          movies = resp.map { |m| m['title'] }
          expect(resp.size).to eq 8
          expect(movies).to include('Santa with Muscles (1996)')
          expect(movies).to include('Someone Else\'s America (1995)')
          expect(movies).to include('Saint of Fort Washington, The (1993)')
          expect(movies).to include('Prefontaine (1997)')
          expect(movies).to include('Marlene Dietrich: Shadow and Light (1996) ')
          expect(movies).to include('Entertaining Angels: The Dorothy Day Story (1996)')
          expect(movies).to include('They Made Me a Criminal (1939)')
          expect(movies).to include('Star Kid (1997)')
        end
      end
    end
  end

  def create_movie(body)
    req = {
      path: '/v1/movies/create',
      body: body.to_json,
      head: { 'content-type' => 'application/json' }
    }
    err = proc { fail 'API request failed' }

    # second argument is callback
    post_request(req, err) do |c|
      expect(c.state).to eq :finished
      resp = JSON.parse(c.response)
      expect(c.response_header.status).to be <= 201
      yield resp if block_given?
    end
  end

  context 'adding new movie' do
    it 'creates new movie' do
      with_api(Movies::App) do
        body = { movie: { title: 'pelisky' } }
        create_movie(body) do |resp|
          expect(resp).to include 'title'
          expect(resp['title']).to eq 'pelisky'
        end
      end
    end

    it 'creates new movie, and make it drama' do
      with_api(Movies::App) do
        body = { movie: { title: 'pelisky', drama: true } }
        create_movie(body) do |resp|
          expect(resp['title']).to eq 'pelisky'
          expect(resp['drama']).to eq true
        end
      end
    end
  end

end

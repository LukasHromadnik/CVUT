require 'yajl/json_gem'
require 'grape'

module Movies
  class MoviesAPI < Grape::API
    resource 'movies' do
      params do
        optional :limit, default: 20, type: Integer
        optional :offset, default: 0, type: Integer
      end
      get '/' do
        Movie.all
             .order(id: :asc)
             .limit(params[:limit])
             .offset(params[:offset])
      end

      #Other urls
    end
  end
end

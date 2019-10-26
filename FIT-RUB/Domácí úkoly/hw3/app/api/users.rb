require 'yajl/json_gem'
require 'grape'

module Movies
  # Users
  #  managing users who submitted ratings of movies
  #
  class UsersAPI < Grape::API
    helpers do
      def user
        User.find(params['id'])
      end
    end

    resource 'users' do
      params do
        optional :limit, default: 20, type: Integer
        optional :offset, default: 0, type: Integer
      end
      get '/' do
        User.all
            .limit(params[:limit])
            .offset(params[:offset])
      end

      #Other urls
    end
  end
end

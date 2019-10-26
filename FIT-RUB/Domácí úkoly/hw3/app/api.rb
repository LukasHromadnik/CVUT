require 'grape'

module Movies
  class API < Grape::API
    version 'v1', using: :path
    format :json
    mount ::Movies::MoviesAPI
    mount ::Movies::OccupationAPI
    mount ::Movies::UsersAPI
    mount ::Movies::StressAPI
  end
end

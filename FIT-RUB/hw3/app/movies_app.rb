require 'goliath'
require 'em-synchrony/em-http'
require 'em-synchrony/activerecord'

module Movies
  class App < Goliath::API
    # parse & merge query and body parameters
    use Goliath::Rack::Params
    # cleanup accepted media types
    use Goliath::Rack::DefaultMimeType
    # JSON output formatter
    # use Goliath::Rack::Formatters::JSON
    # auto-negotiate response format
    use Goliath::Rack::Render
    # respond to /status with 200, OK (monitoring, etc)
    use Goliath::Rack::Heartbeat
    use Goliath::Rack::Validation::RequestMethod, %w(GET POST PUT DELETE)

    def response(env)
      Movies::API.call(env)
    end
  end
end

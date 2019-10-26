curr_dir = File.join(File.dirname(__FILE__), '..', 'app')
$LOAD_PATH.unshift(File.expand_path(curr_dir))
$LOAD_PATH.unshift(File.expand_path(curr_dir, 'api'))
$LOAD_PATH.unshift(File.expand_path(curr_dir, 'models'))
$LOAD_PATH.unshift(File.dirname(__FILE__))
require 'uri'
require 'em-synchrony/activerecord'
require 'yaml'
require 'erb'
require 'boot'

# Bundler.require :default, ENV['RACK_ENV']
# load models
Dir[File.expand_path('../../app/models/*.rb', __FILE__)].each do |f|
  require f
end
# load API files
Dir[File.expand_path('../../app/api/*.rb', __FILE__)].each do |f|
  require f
end

require 'goliath'
require 'api'
require 'movies_app'

I18n.config.enforce_available_locales = false
# Sets up database configuration
db = URI.parse(ENV['DATABASE_URL'] || 'http://localhost')
if db.scheme == 'postgres' # server environment
  ActiveRecord::Base.establish_connection(
    adapter: db.scheme == 'postgres' ? 'em_postgresql' : db.scheme,
    host: db.host,
    username: db.user,
    password: db.password,
    database: db.path[1..-1],
    encoding: 'utf8'
  )
else # local environment
  environment = ENV['DATABASE_URL'] ? 'production' : 'development'
  db = YAML.load(ERB.new(File.read('config/database.yml')).result)[environment]
  ActiveRecord::Base.establish_connection(db)
end

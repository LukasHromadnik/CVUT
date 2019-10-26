
require 'rubygems'
require 'em-synchrony/em-http'
require 'goliath/test_helper'
require 'yajl/json_gem'
require 'rack/test'
require 'factory_girl'
ENV['RACK_ENV'] ||= 'test'

Goliath.env = :test

require File.expand_path('../../config/environment', __FILE__)

RSpec.configure do |config|
  # ## Mock Framework
  #
  # config.mock_with :mocha
  # config.mock_with :flexmock
  # config.mock_with :rr
  config.mock_with :rspec
  config.expect_with :rspec

  # Run specs in random order to surface order dependencies. If you find an
  # order dependency and want to debug it, you can fix the order by providing
  # the seed, which is printed after each run.
  #     --seed 1234
  config.order = 'random'

  config.include Goliath::TestHelper, file_path: /spec\/api/

  Dir[File.expand_path('spec/support/**/*.rb')].each do |f|
    require f
  end
end

puts "environment #{ENV['RACK_ENV']}"
FactoryGirl.reload

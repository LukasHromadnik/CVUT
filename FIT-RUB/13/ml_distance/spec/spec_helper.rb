$LOAD_PATH.unshift(File.dirname(__FILE__))
$LOAD_PATH.unshift(File.join(File.dirname(__FILE__), '..', 'lib'))
require 'ml_distance'
require 'rspec'

RSpec.configure do |c|
  c.color = true
  c.formatter = :documentation
  c.tty = true
end

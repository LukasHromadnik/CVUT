require 'rubygems'
require 'spork'
require 'rspec'
require_relative '../eliza_server'
# uncomment the following line to use spork with the debugger
# require 'spork/ext/ruby-debug'

Spork.prefork do
  # Loading more in this block will cause your tests to run faster. However,
  # if you change any configuration or code from libraries loaded here, you'll
  # need to restart spork for it take effect.

  RSpec.configure do |c|
    c.color = true
    c.formatter = :documentation
    c.tty = true
  end

  Thread.new do
    @server = ElizaServer.new(1096)
    @server.audit = true
    @server.start.join
  end

end

Spork.each_run do
  # This code will be run each time you run your specs.

end

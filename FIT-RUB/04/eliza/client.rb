require_relative 'eliza_client'
require 'socket'

cl = ElizaClient.new('localhost', 1096)
puts cl.say 'Hello'

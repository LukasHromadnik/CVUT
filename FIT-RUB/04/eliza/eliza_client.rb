#!/bin/ruby
require 'socket'

# Simple reuseable socket client
module SocketClient

  def connect(host, port)
    sock = TCPSocket.new(host, port)
    begin
      result = yield sock
    ensure
      sock.close
    end
    result
  rescue Errno::ECONNREFUSED
  end

end

# Should be able to connect to ElizaServer
class ElizaClient
  extend SocketClient

  attr_reader :host, :port

  def initialize(host, port)
    @host = host
    @port = port
  end

  def say(what)
    puts "client: connecting to #{@host}:#{@port}"
    ElizaClient.connect(@host, @port) do |sock|
      sock.puts(what)
      return sock.readline.strip
    end
  end

end
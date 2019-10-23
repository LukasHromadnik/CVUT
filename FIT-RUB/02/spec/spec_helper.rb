#!/usr/bin/env ruby
require 'rspec'

RSpec.configure do |c|
  c.color = true
  c.formatter = :documentation
  c.tty = true
end

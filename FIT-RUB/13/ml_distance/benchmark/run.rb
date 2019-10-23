#!/bin/ruby

require_relative 'distance'
data = Array.new(100_000) { Array.new(10) { rand } }
run(data)

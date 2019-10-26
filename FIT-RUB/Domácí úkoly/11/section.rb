#!/usr/bin/env ruby
require_relative 'content_type'

class Section < ContentType

  def initialize(name)
    @name = name
  end

  def render
    "\\section\{#{@name}\}"
  end

end

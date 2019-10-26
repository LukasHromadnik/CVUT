#!/usr/bin/env ruby
require_relative 'content_type'

class Slide < ContentType

  def initialize(&block)
    instance_eval &block
  end

  def title(text)
    @title = text
  end

  def item(text)
    check_items
    @items << text
  end

  def render
    output = []
    output << "\\begin\{frame\}\{#{@title}\}"
    output << '\begin{itemize}'

    @items.each do |item|
      output << "\\item #{item}"
    end
    output << '\end{itemize}'

    output.join("\n")
  end

  private
  def check_items
    if @items.nil?
      @items = []
    end
  end

end

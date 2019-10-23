#!/usr/bin/env ruby
require_relative 'slide'
require_relative 'section'

class Presentation

  def initialize(block)
    instance_eval block
  end

  def num_slides
    @content.select { |item| item.kind_of? Slide }.count
  end

  def title(text)
    @title = text
  end

  def author(text)
    @author = text
  end

  def theme(text)
    @theme = text
  end

  def section(text)
    check_content
    @content << Section.new(text)
  end

  def slide(&block)
    check_content
    @content << Slide.new(&block)
  end

  def to_s
    parts = []
    parts << '\documentclass[12pt]{beamer}'
    parts << '\setbeamertemplate{navigation symbols}{}'

    if !@title.nil?
      parts << "\\title\{#{@title}\}"
    end

    if !@author.nil?
      parts << "\\author\{#{@author}\}"
    end

    if !@theme.nil?
      parts << "\\usetheme\{#{@theme}\}"
    end
      
    parts << '\begin{document}'

    check_content
    @content.each do |part|
      parts << part.render
    end

    parts << '\end{document}'
    parts << nil

    parts.join("\n")
  end

  private
  def check_content
    if @content.nil?
      @content = []
    end
  end
end

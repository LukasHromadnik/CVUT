require 'net/http'
require 'Nokogiri'
require 'open-uri'
require 'openssl'
require 'uri'
require 'rss'

class ChannelsController < ApplicationController

  def index
    @channels = Channel.all
  end

  def new
    @channel = Channel.new
  end

  def create
    url = params[:channel][:link]

    begin
      channel_url = build_url(url)
    rescue
      render plain: "Chyba"
    end

    html = Nokogiri::HTML(open(channel_url))
    rss_elements = html.xpath('//link[@type="application/rss+xml"]')

    links = Array.new

    for item in rss_elements
      link = Link.new
      link.title = item.attribute('title').value
      link.link = item.attribute('href').value
      links << link
    end

    main_feed = links.first

    @channel = Channel.new

    feed = RSS::Parser.parse(open(main_feed.link))
    channel_item = feed.channel
    @channel.title = channel_item.title
    @channel.description = channel_item.description
    @channel.link = channel_item.link
    if !@channel.save
      render 'new'
      return
    end

    feed.items.each do |item|
      article = Article.new
      article.title = item.title
      article.link = item.link
      article.description = item.description
      article.guid = item.guid
      article.channel = @channel
      article.save
    end

    p "ahoj"

    redirect_to channels_path
  end

  def destroy
    @channel = Channel.find(params[:id])
    @channel.destroy

    redirect_to channels_path
  end

  def articles
    @channel = Channel.find(params[:id])
    @articles = @channel.articles
  end

  private

  def build_url(url)
    current_url = url
    parsed_url = URI.parse(current_url)

    has_scheme = true
    has_path = true

    if parsed_url.scheme.nil?
      has_scheme = false
      current_url = "http://" + current_url
      parsed_url = URI.parse(current_url)
    end

    if parsed_url.path.nil? || parsed_url.path.length == 0
      has_path = false
      current_url = current_url + "/"
      parsed_url = URI.parse(current_url)
    end

    response = build_request(parsed_url)

    while response.kind_of? Net::HTTPMovedPermanently
      parsed_url = URI.parse(response.header["location"])
      response = build_request(parsed_url)
    end

    parsed_url.to_s
  end

  def build_request(uri)
    Net::HTTP.start(uri.host, uri.port, :use_ssl => uri.scheme == "https") do |http|
      http.request Net::HTTP::Get.new(uri)
    end
  end
end

class Link
  attr_accessor :title, :link
end
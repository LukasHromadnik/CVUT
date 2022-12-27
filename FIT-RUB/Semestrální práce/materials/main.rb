require 'net/http'
require 'Nokogiri'
require 'open-uri'
require 'uri'
require 'rss'
require 'htmlentities'

class Link
  attr_accessor :title, :link
end

base_url = 'http://jablickar.cz'
# source = Net::HTTP.get(base_url + '/')
# p source
# p fetch base_url
# p source.class
# html = Nokogiri::HTML(open(URI(base_url).to_s))
html = Nokogiri::HTML(open(base_url))
# p html.to_s
# p html
# p URI(base_url)

# p html.xpath('//link')
rss_elements = html.xpath('//link[@type="application/rss+xml"]')
# rss_elements = html.xpath('//html')
# p rss_elements

links = Array.new

for item in rss_elements
  link = Link.new
  link.title = item.attribute('title').value
  link.link = item.attribute('href').value
  links << link
end

main_feed = links.first

# rss = Nokogiri::XML(open(main_feed.link))
# p rss.to_s
# if rss_elements.count > 0
#   href = rss_elements.first.attribute('href').value
#   if href.include? base_url # TODO: Hostname check
#     rss_resource = href
#   else
#     rss_resource = base_url + href
#   end
# end

# p rss_resource

# channel = Channel.new

open(main_feed.link) do |rss|
  feed = RSS::Parser.parse(rss)
  # channel_item = feed.channel
  # channel.title = channel_item.title
  # channel.description = channel_item.description
  # channel.link = channel_item.link
  feed.items.each do |item|
    p string_tags(HTMLEntities.new.decode(item.description))
  end
end

# p channel.to_s
# p rss_resource
# p Net::HTTP.get(rss_resource)
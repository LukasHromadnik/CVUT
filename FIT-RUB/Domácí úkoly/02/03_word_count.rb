#!/usr/bin/env ruby

# Count number of words in a given files
# (regardless font case)
def word_count(file)
  f = File.open(file, "r")
  words = {}
  f.readlines
    .map { |line| line.split(" ") }
    .flatten
    .map { |word| word.gsub(/[^a-z ]/i, '').downcase.to_sym }
    .each { |word| words.has_key?(word) ? words[word] += 1 : words[word] = 1 }
  f.close
  
  words
end

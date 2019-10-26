#!/bin/ruby

def meaning_of_the_world
  42
end

def array_with_three_elements
  [ 1, 2, 3 ]
end

def hash_with_one_key
  { :piano => "piano" }
end

def reverse_arg(thing)
  thing.reverse
end

# hint: the argument should be string,
# have a look at the documentation
# http://ruby-doc.org/core-2.0.0/String.html
def capitalize_arg(text)
  text.upcase
end

# should return 3rd letter of text
# in argument, if doesn't exists return `nil`
def third_elem(thing)
  thing[2]
end

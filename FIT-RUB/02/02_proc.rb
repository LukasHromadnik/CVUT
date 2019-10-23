#!/usr/bin/env ruby

# Create a procedure to uppercase only
# Strings beginning with letter 'a' or 'A'.
# Proc returns original object if not String.
def bring_up
  Proc.new { |value| value[0] == 'a' || value[0] == 'A' ? value.upcase : value }
end

# Create a procedure to multiple only
# values representing an Integer by two.
# Proc returns a string representation.
# Proc returns original string if not Integer-like.
def make_double
  Proc.new do |value|
    if value.is_a?(Integer)
      (value * 2).to_s
    elsif value.is_a?(String) && value.to_i != 0
      (value.to_i * 2).to_s
    else
      value
    end
  end
end

# Create a function to multiply all
# numbers in text by two.
# (Hint: use the previously defined procedure)
# Set of whitespaces should be changed to one space.
# (Hint: split and join is ok)

def increase_wage(contract)
  contract.split(" ").map(&make_double).join(" ")
end

# Run both of the created procedures
# and collect resulting array.

def process_text_array(array)
  array.map(&bring_up).map(&make_double)
end

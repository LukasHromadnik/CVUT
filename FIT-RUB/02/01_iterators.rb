#!/usr/bin/env ruby

# Return array of odd elements from `array`
# in case that that a block is given, yield
# elements on odd index
# e.g.: [1,2,3,4,5,6] -> [2,4,6]
def odd_elements(array)
  result = array.each_index.select(&:odd?).map { |index| array[index] }
 
  if block_given?
    result.map! { |item| yield item }
  end
  
  result
end

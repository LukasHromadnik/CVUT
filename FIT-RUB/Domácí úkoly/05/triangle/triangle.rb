# Triangle Project Code.

# Triangle analyzes the lengths of the sides of a triangle
# (represented by a, b and c) and returns the type of triangle.
#
# It returns:
#   :equilateral  if all sides are equal
#   :isosceles    if exactly 2 sides are equal
#   :scalene      if no sides are equal
#
def triangle(a, b, c)
  sides = [a, b, c].sort!
  
  if sides[0] + sides[1] <= sides[2]
    raise TriangleError, "Sum of two sides must be greater than the last side"
  end

  if a == b && b == c
    :equilateral
  elsif a == b || b == c
    :isosceles
  else
    :scalene
  end
end

# Error class used in part 2.  No need to change this code.
class TriangleError < StandardError
end

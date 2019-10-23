#include <ruby.h>
#include <math.h>

/*
module MlDistance
  def euclidean(other)
    sum = 0.0
    self.each_index do |i|
      sum += (self[i] - other[i])**2
    end
    Math.sqrt(sum)
  end
end
*/
static VALUE rb_euclidean(VALUE self, VALUE other_array) {
  double value = 0.0;
  unsigned int index;
  unsigned long vector_length = (RARRAY_LEN(self) - 1);
  unsigned long other_length = (RARRAY_LEN(other_array) - 1);

  //TODO: check vectors are the same size

  for(index = 0; index <= vector_length; index++) {
    double x, y;

    x = NUM2DBL(RARRAY_PTR(self)[index]);
    y = NUM2DBL(RARRAY_PTR(other_array)[index]);

    value += pow(x - y, 2);
  }

  return rb_float_new(sqrt(value));
}

void Init_ml_distance() {
  VALUE ml_distance = rb_define_module("MlDistance");
  rb_define_method(ml_distance, "euclidean", rb_euclidean, 1);
}

# MlDistance

Native Ruby extension for computing Machine Learning distances.


## Usage

```ruby
[1, 6].euclidean([4, 2])
# => 5.0
```

## Installation

install required gems:

    $ bundle install
    
compile extension:

    $ rake compile
    
build gem and install gem

    $ rake install

## Benchmark

    $ ruby benchmark/run.rb


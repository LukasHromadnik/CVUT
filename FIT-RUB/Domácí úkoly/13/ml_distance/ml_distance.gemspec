# coding: utf-8
lib = File.expand_path('../lib', __FILE__)
$LOAD_PATH.unshift(lib) unless $LOAD_PATH.include?(lib)
require 'ml_distance/version'
require 'rake/extensiontask'

Gem::Specification.new do |s|
  s.name          = 'ml_distance'
  s.version       = MlDistance::VERSION
  s.authors       = ['Tomas Barton']
  s.email         = ['barton.tomas@gmail.com']
  s.description   = 'Distance measures for machine learning'
  s.summary       = 'Basic implementation of distance measures'

  s.homepage      = ''
  s.license       = 'MIT'
  s.files         = `git ls-files`.split($INPUT_RECORD_SEPARATOR)
  s.executables   = s.files.grep(%r{^bin/}) { |f| File.basename(f) }
  s.test_files    = s.files.grep(%r{^(test|spec|features)/})
  s.require_paths = ['lib']

  s.platform = Gem::Platform::RUBY
  s.extensions = FileList['ext/**/extconf.rb']
  s.add_development_dependency 'bundler', '~> 1.3'
  s.add_development_dependency 'rake', '> 10.0'
  s.add_development_dependency 'rake-compiler'
  s.add_development_dependency 'rspec'
end

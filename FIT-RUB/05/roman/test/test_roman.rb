require 'minitest/unit'
require 'minitest/autorun'
require_relative '../lib/roman'

class TestRoman < MiniTest::Unit::TestCase
  def test_simple
    assert_equal('i', Roman.new(1).to_s)
    assert_equal('ii', Roman.new(2).to_s)
    assert_equal('iii', Roman.new(3).to_s)
    assert_equal('iv', Roman.new(4).to_s)
    assert_equal('ix', Roman.new(9).to_s)
    assert_equal('lvi', Roman.new(56).to_s)
    assert_equal('cdlvi', Roman.new(456).to_s)
    assert_equal('mmmcmxcix', Roman.new(3999).to_s)
    assert_equal('mmmdccclxxxviii', Roman.new(3888).to_s)
  end
end

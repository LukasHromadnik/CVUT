require 'test_helper'

class ThesesControllerTest < ActionDispatch::IntegrationTest
  setup do
    @thesis = theses(:one)
  end

  test "should get index" do
    get theses_url
    assert_response :success
  end

  test "should get new" do
    get new_thesis_url
    assert_response :success
  end

  test "should create thesis" do
    assert_difference('Thesis.count') do
      post theses_url, params: { thesis: { abstract: @thesis.abstract, defended: @thesis.defended, title: @thesis.title } }
    end

    assert_redirected_to thesis_url(Thesis.last)
  end

  test "should show thesis" do
    get thesis_url(@thesis)
    assert_response :success
  end

  test "should get edit" do
    get edit_thesis_url(@thesis)
    assert_response :success
  end

  test "should update thesis" do
    patch thesis_url(@thesis), params: { thesis: { abstract: @thesis.abstract, defended: @thesis.defended, title: @thesis.title } }
    assert_redirected_to thesis_url(@thesis)
  end

  test "should destroy thesis" do
    assert_difference('Thesis.count', -1) do
      delete thesis_url(@thesis)
    end

    assert_redirected_to theses_url
  end
end

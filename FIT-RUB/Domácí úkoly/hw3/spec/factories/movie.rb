# include ActionDispatch::TestProcess

FactoryGirl.define do
  factory :movie, class: Movie do
    id 9999
    title { 'The Matrix' }
  end
end

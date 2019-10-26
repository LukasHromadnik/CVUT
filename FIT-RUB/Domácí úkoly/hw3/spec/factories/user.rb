
FactoryGirl.define do
  factory :user, class: User do
    id 9999
    age { 21 }
    gender { 'M' }
    zip { '123456' }
  end
end

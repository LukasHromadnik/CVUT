class Thesis < ApplicationRecord
  belongs_to :faculty
  belongs_to :person, optional: true
 
  validates :title, presence: true
end

class Channel < ApplicationRecord
  validates :link, uniqueness: true

  has_many :articles, dependent: :destroy
end

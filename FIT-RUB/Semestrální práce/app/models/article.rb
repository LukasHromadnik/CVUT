class Article < ApplicationRecord
  validates :guid, uniqueness: true

  belongs_to :channel
end

class AddChannelIdToArticle < ActiveRecord::Migration[5.0]
  def change
    add_reference :articles, :channel, foreign_key: true
  end
end

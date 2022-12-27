class AddGuidToArticles < ActiveRecord::Migration[5.0]
  def change
    add_column :articles, :guid, :string
  end
end

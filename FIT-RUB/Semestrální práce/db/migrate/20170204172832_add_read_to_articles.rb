class AddReadToArticles < ActiveRecord::Migration[5.0]
  def change
    add_column :articles, :read, :integer, :default => 0
  end
end

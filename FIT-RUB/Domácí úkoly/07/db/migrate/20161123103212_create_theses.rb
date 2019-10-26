class CreateTheses < ActiveRecord::Migration[5.0]
  def change
    create_table :theses do |t|
      t.string :title
      t.text :abstract
      t.date :defended

      t.timestamps
    end
  end
end

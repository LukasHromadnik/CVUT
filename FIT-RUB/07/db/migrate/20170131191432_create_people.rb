class CreatePeople < ActiveRecord::Migration[5.0]
  def change
    create_table :people do |t|
      t.string :username
      t.string :name
      t.string :surname

      t.timestamps
    end
  end
end

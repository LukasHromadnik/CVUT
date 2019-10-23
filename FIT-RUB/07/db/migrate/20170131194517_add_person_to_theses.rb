class AddPersonToTheses < ActiveRecord::Migration[5.0]
  def change
    add_column :theses, :person_id, :integer
    add_index :theses, :person_id
  end
end

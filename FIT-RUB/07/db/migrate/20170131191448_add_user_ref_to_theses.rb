class AddUserRefToTheses < ActiveRecord::Migration[5.0]
  def change
    add_reference :theses, :user, foreign_key: true
  end
end

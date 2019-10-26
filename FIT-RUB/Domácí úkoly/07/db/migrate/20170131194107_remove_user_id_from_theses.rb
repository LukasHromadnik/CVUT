class RemoveUserIdFromTheses < ActiveRecord::Migration[5.0]
  def change
    remove_column :theses, :user_id, :integer
  end
end

class AddFacultyIdToTheses < ActiveRecord::Migration[5.0]
  def change
    add_reference :theses, :faculty, index: true, foreign_key: true
  end
end

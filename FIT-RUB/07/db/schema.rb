# This file is auto-generated from the current state of the database. Instead
# of editing this file, please use the migrations feature of Active Record to
# incrementally modify your database, and then regenerate this schema definition.
#
# Note that this schema.rb definition is the authoritative source for your
# database schema. If you need to create the application database on another
# system, you should be using db:schema:load, not running all the migrations
# from scratch. The latter is a flawed and unsustainable approach (the more migrations
# you'll amass, the slower it'll run and the greater likelihood for issues).
#
# It's strongly recommended that you check this file into your version control system.

ActiveRecord::Schema.define(version: 20170131194517) do

  create_table "faculties", force: :cascade do |t|
    t.string   "name"
    t.integer  "code"
    t.datetime "created_at", null: false
    t.datetime "updated_at", null: false
  end

  create_table "people", force: :cascade do |t|
    t.string   "username"
    t.string   "name"
    t.string   "surname"
    t.datetime "created_at", null: false
    t.datetime "updated_at", null: false
  end

  create_table "theses", force: :cascade do |t|
    t.string   "title"
    t.text     "abstract"
    t.date     "defended"
    t.datetime "created_at", null: false
    t.datetime "updated_at", null: false
    t.integer  "faculty_id"
    t.integer  "person_id"
    t.index ["faculty_id"], name: "index_theses_on_faculty_id"
    t.index ["person_id"], name: "index_theses_on_person_id"
  end

end

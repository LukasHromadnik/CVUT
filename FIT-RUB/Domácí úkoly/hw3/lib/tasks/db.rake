
namespace :db do

  # Before creating the database we have to connect to
  # global postgres namespace
  task :load_global do
    ActiveRecord::Base.configurations = db_conf

    # drop and create need to be performed with a connection
    # global namespace
    conf = db_conf[env].merge(
      'database' => 'postgres',
      'schema_search_path' => 'public'
    )
    ActiveRecord::Base.establish_connection conf
  end

  desc 'creates and migrates your database'
  task setup: :load_global do
    Rake::Task['db:create'].invoke
    Rake::Task['db:migrate'].invoke
  end

  task :connect do
    puts "environment: #{env}"
    ActiveRecord::Base.configurations = db_conf
    ActiveRecord::Base.establish_connection db_conf[env]
  end

  desc 'migrate your database'
  task migrate: :connect do
    ActiveRecord::Migrator.migrate(
      ActiveRecord::Migrator.migrations_paths,
      ENV['VERSION'] ? ENV['VERSION'].to_i : nil
    )
  end

  desc 'Drops the database'
  task drop: :load_global do
    ActiveRecord::Base.connection.drop_database db_conf[env]['database']
  end
  desc 'Creates the database'
  task create: :load_global do
    ActiveRecord::Base.connection.create_database db_conf[env]['database']
  end

  desc 'Reset primary key of all tables'
  task reset_pk: :connect do
    ActiveRecord::Base.connection.tables.each do |t|
      puts "reseting sequence of #{t}"
      ActiveRecord::Base.connection.reset_pk_sequence!(t)
    end
  end
end

def env
  ENV['RACK_ENV'] ? ENV['RACK_ENV'] : 'development'
end

def db_conf
  YAML.load(ERB.new(File.read('config/database.yml')).result)
end

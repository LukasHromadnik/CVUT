# Rake tasks for importing move lens dataset

def download(url, dest)
  File.open(dest, 'wb') do |fo|
    fo.write open(url).read
  end
  puts "downloaded #{url}"
end

def mkdir(dir)
  Dir.mkdir(dir) unless File.exist?(dir)
end

def extract(target, tmp)
  puts "target: #{target}"
  temp_dir = File.expand_path File.join(File.dirname(__FILE__), "../../#{tmp}")
  if File.exist?(target)
    Zip::File.open(target) do |zf|
      zf.each do |e|
        zf.extract(e.name, File.join(temp_dir.to_s, e.name))
      end
    end
  else
    exit("file #{target} not found")
  end
  temp_dir
end

def invoke(task, *args)
  Rake::Task[task].invoke(*args)
end

def read_content(path, file)
  data = "#{path}/#{file}"
  IO.read(data).force_encoding('ISO-8859-1').encode('utf-8', replace: nil)
end

def import_data(data, model, mapping, separator)
  data.split("\n").each do |line|
    row = line.split(separator)
    t = model.new
    mapping.each_with_index do |key, idx|
      t.send("#{key}=", row[idx])
    end
    t.save!
  end
  import_msg(model)
end

def import_msg(model)
  puts "imported #{model.count} #{model.to_s.downcase.pluralize}"
end

def to_db_name(table)
  table.capitalize.constantize
end

def dataset_url(dataset)
  "http://files.grouplens.org/datasets/movielens/#{dataset}.zip"
end

namespace :data do
  desc 'downloads basic 100k dabaset'
  task :fetch do
    dir, dataset = 'tmp', 'ml-100k'
    Dir.mkdir(dir) unless File.exist?(dir)
    target = 'tmp/data.zip'
    download dataset_url(dataset), target unless File.exist?(target)
    dest = extract(target, 'tmp') unless Dir.exist?("#{dir}/#{dataset}")
    puts "dir: #{dest}"
    invoke('data:import')
    Rake::Task['db:reset_pk'].invoke
  end

  desc 'import data from tmp dir'
  task :import do
    dir, dataset = 'tmp', 'ml-100k'
    invoke('db:connect')
    path = "#{dir}/#{dataset}"
    %w(movie occupation user rating).each do |table|
      invoke("data:#{table.pluralize}", path) unless to_db_name(table).count > 0
    end
  end

  task :movies, :path  do |_t, args|
    content = read_content(args[:path], 'u.item')
    mapping = [:id, :title, :release_date, :video_release_date, :imdb_url,
               :unknown, :action, :adventure, :animation, :childrens, :comedy,
               :comedy, :crime, :documentary, :drama, :fantasy, :film_noir,
               :horror, :musical, :mystery, :romance, :scifi, :thriller,
               :war, :western
              ]
    import_data(content, Movie, mapping, '|')
  end

  task :occupations, :path  do |_t, args|
    content = read_content(args[:path], 'u.occupation')
    mapping = [:title]
    import_data(content, Occupation, mapping, '|')
  end

  task :users, :path  do |_t, args|
    content = read_content(args[:path], 'u.user')
    mapping = [:id, :age, :gender, :occupation, :zip]
    import_data(content, User, mapping, '|')
  end

  task :ratings, :path  do |_t, args|
    content = read_content(args[:path], 'u1.base')
    mapping = [:user_id, :movie_id, :rating, :rated_at]
    import_data(content, Rating, mapping, "\t")
  end
end

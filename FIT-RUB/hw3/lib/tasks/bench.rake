# simple benchmark
require 'em-http-request'

namespace :bench do
  desc 'basic user-knn benchmark'
  task :knn do
    Rake::Task['db:connect'].invoke
    server = 'http://localhost:5000/v1/'
    puts "users: #{User.all.size}"
    total, i = 0, 0

      User.all.each do |u|
        start = Time.now
        EventMachine.run {
          http = EventMachine::HttpRequest.new("#{server}/users/#{u.id}/knn").get
          http.errback { p 'Uh oh'; EM.stop }
          http.callback {
            time = Time.now - start
            total += time
            i += 1
            EventMachine.stop
            puts "%3d: time: %.2f, avg %.2f, #{http.response}" % [ u.id, time, total / i ]
          }
        }
      end
  end
end
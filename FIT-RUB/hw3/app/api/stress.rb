require 'yajl/json_gem'
require 'grape'

module Movies
  class StressAPI < Grape::API
    resource 'stress' do
      get '/pretend' do
        # pretend some expensive query with sleeping for 1s
        User.find_by_sql('select pg_sleep(1)')
        # this is enough for returning JSON response
        { text: 'Oh hey' }
      end
    end
  end
end

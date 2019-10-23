require_relative 'eliza_server'

server = ElizaServer.new(1096)
server.audit = true                  # Turn logging on.
server.start.join

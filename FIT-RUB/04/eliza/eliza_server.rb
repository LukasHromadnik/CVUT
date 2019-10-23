require 'gserver'

class ElizaServer < GServer

  def initialize(port)
    super(port)
    @request_cnt = 0
  end

  def serve(io)
    @request_cnt += 1
    log "request \##{@request_cnt}"
    loop do
      line = io.readline.strip
      log "recv: '#{line.strip}'"
      reply = eliza_logic(line)
      log "repl: '#{reply}'"
      io.puts reply
    end
  end

  protected

  def eliza_logic(input)
    data = {
      'hello' => 'Hello. My name is Eliza. How may I help you?',
      'ache' => 'Maybe you should consult a doctor of medicine.',
      'problem' => 'Say, do you have any psychological problems?',
      'notreally' => 'Are you sure?',
      'mymother' => 'You mentioned your mother, how\'s your relationship?',
      'ithink' => 'Do you really think so?',
      'doyou' => 'We were discussing you, not me.'
    }

    word = input.downcase.gsub(/[^0-9a-z]/i, '')
    keys = data.keys.select { |item| word.include? item }

    if !keys.empty?
      data[keys.first]
    else
      'I don\'t know what are you talking about.'
    end
  end

end

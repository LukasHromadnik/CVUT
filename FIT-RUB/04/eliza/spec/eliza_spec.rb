require 'rspec'
require_relative 'spec_helper'
require_relative '../eliza_server'
require_relative '../eliza_client'

describe 'Elisa server' do

  subject { ElizaClient.new('localhost', port) }
  let(:port) { 1096 }

  it 'uses module' do
    expect(ElizaClient.const_defined?(:SocketClient)).to be true
    expect(ElizaClient.methods.include?(:connect)).to be true
  end

  describe 'client connection' do
    it 'accepts client connections' do
      expect(
        subject.say 'Hello'
      ).to eq 'Hello. My name is Eliza. How may I help you?'
    end

    it 'ignores font CaSE' do
      expect(
        subject.say 'hello'
      ).to eq 'Hello. My name is Eliza. How may I help you?'
      expect(
        subject.say 'HELLO'
      ).to eq 'Hello. My name is Eliza. How may I help you?'
    end
  end

  describe 'simple conversation' do
    it 'provides reasonable answers' do
      expect(
        subject.say 'Hello'
      ).to eq 'Hello. My name is Eliza. How may I help you?'
      expect(
        subject.say 'I have a terrible headache'
      ).to eq 'Maybe you should consult a doctor of medicine.'
      expect(
        subject.say 'I have a terrible stomachache'
      ).to eq 'Maybe you should consult a doctor of medicine.'
      expect(
        subject.say 'The toothache is unbearable'
      ).to eq 'Maybe you should consult a doctor of medicine.'
    end
  end

  describe 'ask for details' do
    it 'provides reasonable answers' do
      expect(
        subject.say 'I have a problem'
      ).to eq 'Say, do you have any psychological problems?'
      expect(
        subject.say 'No, not really'
      ).to eq 'Are you sure?'
    end

    it 'looks for keywords' do
      expect(
        subject.say 'My mother always said...'
      ).to eq 'You mentioned your mother, how\'s your relationship?'
      expect(
        subject.say 'I think it is fine'
      ).to eq 'Do you really think so?'
      expect(
        subject.say 'That is the problem'
      ).to eq 'Say, do you have any psychological problems?'
      expect(
        subject.say 'Not really'
      ).to eq 'Are you sure?'
    end
  end

  describe 'questions concerting psychologist' do
    it 'focuses on client' do
      expect(
        subject.say 'Do you have time tomorrow'
      ).to eq 'We were discussing you, not me.'
      expect(
        subject.say 'I think we need to meet again'
      ).to eq 'Do you really think so?'
      expect(
        subject.say 'What do you recommend?'
      ).to eq 'We were discussing you, not me.'
      expect(
        subject.say 'What do you think?'
      ).to eq 'We were discussing you, not me.'
    end

  end

end

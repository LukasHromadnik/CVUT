# -*- encoding : utf-8 -*-
require_relative 'spec_helper'
require_relative '../table_tennis_scorer'

describe TableTennisScorer do

  let(:scorer) { TableTennisScorer.new }

  describe '#score' do

    subject { scorer.score }

    context 'at the beginning' do
      it { is_expected.to eql '0-0' }
    end

    context 'server wins a point' do
      before { scorer.player1.scores }
      it { is_expected.to eql '1-0' }
    end

    context 'receiver wins a point' do
      before { scorer.player2.scores }
      it { is_expected.to eql '0-1' }
    end

    context 'both win a point' do
      before do
        scorer.player1.scores
        scorer.player2.scores
      end
      it { is_expected.to eql '1-1' }
    end
    
    context 'server wins a game' do
      before do
        10.times { scorer.player1.scores }
        7.times { scorer.player2.scores }
        scorer.player1.scores
      end
      it { is_expected.to eql 'server game' }
    end
    
    context 'receiver wins a game' do
      before do
        7.times { scorer.player1.scores }
        10.times { scorer.player2.scores }
        scorer.player2.scores
      end
      it { is_expected.to eql 'receiver game' }
    end
    
    context 'two point lead after deuce' do
      before do
        10.times do
          scorer.player1.scores
          scorer.player2.scores
        end
        scorer.player1.scores
      end

      it { is_expected.to eql '11-10' }

      it do
        scorer.player2.scores
        is_expected.to eql '11-11'
      end

      it do
        scorer.player1.scores
        scorer.player2.scores
        is_expected.to eql '12-11'
      end

      it do
        scorer.player1.scores
        scorer.player1.scores
        scorer.player2.scores
        is_expected.to eql 'server game'
      end
    end
    
  end
end
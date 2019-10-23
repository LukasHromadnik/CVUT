#!/usr/bin/env ruby

$LOAD_PATH << './'
require 'rspec'
require_relative 'spec_helper'
require 'cell'

describe Cell do
  context 'with predefined values' do
    subject(:cell) { Cell.new(1, 9) }

    it { expect(cell.value).to eq 1 }
    it { expect(cell.filled?).to eq true }
    it { expect(cell.num_possible).to eq(-1) }
    it { expect(cell.to_i).to eq 1 }
  end

  context 'without value' do
    subject(:cell) { Cell.new(0, 9) }

    it { expect(cell.value).to eq 0 }
    it { expect(cell.filled?).to eq false }
    it { expect(cell.num_possible).to eq 9 }
    it { expect(cell.to_i).to eq 0 }

    it 'will exclude value' do
      expect { cell.exclude(5) }.to change { cell.num_possible }.from(9).to(8)
    end
  end

end

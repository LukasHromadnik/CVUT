#!/usr/bin/env ruby
require_relative '../presentation'
require_relative 'spec_helper'

describe Presentation do

  subject(:struct) { described_class.new(dsl) }
  let(:dsl) {}

  context 'empty document' do
    let(:dsl) { '' }

    it 'generates LaTeX' do
      expect(subject.to_s).to match /^\\documentclass/
      expect(subject.to_s).to match /^\\begin{document}/
      expect(subject.to_s).to match /^\\end{document}$/
      expect(subject.num_slides).to eq 0
    end
  end

  context 'basic attributes' do
    let(:dsl) do
      <<-EOS
        title 'Habbits'
        author 'John Doe'
        theme 'warsaw'
      EOS
    end

    it 'generates LaTeX' do
      expect(subject.to_s).to match /^\\documentclass/
      expect(subject.to_s).to match /^\\begin{document}/
      expect(subject.to_s).to match /^\\end{document}$/
      expect(subject.num_slides).to eq 0
    end

    it 'sets document attributes' do
      expect(subject.instance_variable_get(:@title)).to eq 'Habbits'
      expect(subject.instance_variable_get(:@author)).to eq 'John Doe'
      expect(subject.instance_variable_get(:@theme)).to eq 'warsaw'
    end

    it 'creates correct title page' do
      expect(subject.to_s).to match /^\\title{Habbits}/
      expect(subject.to_s).to match /^\\author{John Doe}/
    end

    it 'sets correct theme' do
      expect(subject.to_s).to match /^\\usetheme{warsaw}/
    end
  end

  context 'extended' do
    let(:dsl) do
      <<-EOS
        title 'Habbits'
        author 'John Doe'
        theme 'warsaw'

        section 'In the morning'
        slide do
          title 'Getting up'
          item 'Turn off alarm'
          item 'Get out of bed'
        end
      EOS
    end

    it 'generates slides' do
      expect(subject.num_slides).to eq 1
    end

    it 'creates slide' do
      expect(subject.to_s).to match /\\begin{frame}/
    end

    it 'fills slide title' do
      expect(subject.to_s).to match /\\begin{frame}{Getting up}/
    end

    it 'creates section in correct place' do
      expect(subject.to_s).to match /\\section{In the morning}\s*\\begin{frame}/
    end

    it 'fills slide items' do
      expect(subject.to_s).to match /\\begin{itemize}\s*\\item Turn off alarm\s*\\item Get out of bed/
    end
  end
end

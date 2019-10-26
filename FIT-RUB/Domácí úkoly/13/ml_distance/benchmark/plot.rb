require 'benchmark'
require 'gnuplot'

require_relative 'distance'

data = Array.new(100_000) { Array.new(10) { rand } }

bench = run(data)

def plot(x, *ys)
  Gnuplot.open do |gp|
    Gnuplot::Plot.new(gp) do |plot|
      ys.each do |y|
        plot.data << Gnuplot::DataSet.new([x, y]) do |ds|
          ds.with = 'linespoints'
          ds.notitle
        end
      end
    end
  end
end

Gnuplot.open do |gp|
  Gnuplot::Plot.new(gp) do |plot|
    plot.terminal "pdf font 'Georgia,8'"
    plot.title "Euclidean distance 1 to N - array #{data.size} x #{data[0].size}"
    # plot.style  "data histograms"
    # plot.xtics  "nomirror rotate by -45"
    plot.boxwidth '0.5'
    plot.style 'fill solid border -1'
    plot.key 'off'
    plot.output 'benchmark.pdf'

    i = 0
    bench.each do |key, value|
      x = ["#{i} \"#{key}\" #{value.real}"]
      i += 1
      plot.data << Gnuplot::DataSet.new([x]) do |ds|
        ds.using = '1:3:xtic(2)'
        ds.with = 'boxes'
      end
    end
  end
end

reset
set encoding utf8

set multiplot layout 1,2
	
	set timefmt "%H:%M:%S"
	set xdata time
	set xtics format "%H:%M:%S"
	set xtics rotate by 90 right
	
	set origin 0,0
	set size 0.5,1
	set title "Overview"

	unset ylabel
	unset y2label
	unset ytics
	unset y2tics
	
	set xlabel "Timestamp of measurement"
	
	set key on outside center bottom width 1 height 1 title "Legend" box
	
	plot "res.txt" using 1:2 title "temperature" with line,\
	"res.txt" using 1:3 title "load average per minute" with line,\
	"res.txt" using 1:4 axes x1y2 title "CPU fan speed" with line

	set origin 0.5,0
	set size 0.5,1
	set title "Load vs. temperature"

	set ytics nomirror
	set y2tics nomirror

	set ylabel "temperature [K]"
	set y2label "load"
	
	plot "res.txt" using 1:2 axes x1y1 title "temperature" with linespoints,\
	"res.txt" using 1:3 axes x1y2 title "load average per minute" with linespoints

unset multiplot

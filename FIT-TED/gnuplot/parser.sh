#!/bin/bash
paste data_temp data_temp.txt > data_temp_1.txt
cat data_temp_1.txt | awk '{if (NR % 5 == 0) print $4" "($8+273.15)}' | sed 's/,/./g' > temp.txt
cat data_load.txt | awk '{if (NR % 10 == 0) print $1" "$8}' | cut -d',' -f1,2 | cut -d' ' -f2 | sed 's/,/./g' > load.txt
cat data_fan.txt | awk '{if (NR % 20 == 19) print $2}' > fan.txt
paste -d" " temp.txt load.txt fan.txt > res.txt
rm data_temp_1.txt temp.txt load.txt fan.txt
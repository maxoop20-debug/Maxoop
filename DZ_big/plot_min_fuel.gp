# GNUPLOT script for min fuel trajectory
set terminal pngcairo size 800,600 enhanced
set output 'IL-76_min_fuel.png'

set title 'IL-76 - Minimize Fuel'
set xlabel 'Speed V (km/h)'
set ylabel 'Altitude H (m)'
set grid
set datafile separator ','
plot 'trajectory_min_fuel.csv' using 3:2 every ::1 \
     with linespoints lw 2 pt 7 ps 1.5 lc rgb 'blue' \
     title 'Min Fuel Trajectory'

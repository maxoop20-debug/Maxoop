# GNUPLOT script for min time trajectory
set terminal pngcairo size 800,600 enhanced
set output 'IL-76_min_time.png'

set title 'IL-76 - Minimize Time'
set xlabel 'Speed V (km/h)'
set ylabel 'Altitude H (m)'
set grid
set datafile separator ','
# Plot with points at each vertex
plot 'trajectory_min_time.csv' using 3:2 every ::1 \
     with linespoints lw 2 pt 7 ps 1.5 lc rgb 'red' \
     title 'Min Time Trajectory'

# Optionally label maneuvers
# To add maneuver labels, uncomment below:
# set label 'Razgon' at first point...

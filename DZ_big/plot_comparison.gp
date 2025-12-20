# GNUPLOT script for trajectory comparison
set terminal pngcairo size 1200,800 enhanced font 'Verdana,12'
set output 'IL-76_comparison.png'

set title 'IL-76 Trajectory Comparison (Variant 1)'
set xlabel 'Speed V (km/h)'
set ylabel 'Altitude H (m)'
set grid
set key top left
set datafile separator ','

# Plot with lines AND points
plot 'trajectory_min_time.csv' using 3:2 every ::1 \
     with linespoints lw 2 pt 7 ps 1.2 lc rgb 'red' \
     title 'Min Time (16.3838 min, 3612.33 kg)', \
     'trajectory_min_fuel.csv' using 3:2 every ::1 \
     with linespoints lw 2 pt 7 ps 1.2 lc rgb 'blue' \
     title 'Min Fuel (20.3333 min, 3072.68 kg)'


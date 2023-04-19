from pylab import *
from scipy.interpolate import make_interp_spline

# Define data type and read data
mytype = dtype([("T_Water", "<f4"), ("T_average_of_Al_Block", "<f4"), ("millistime", "<u4")])
result = fromfile("Temp_Rec.DAT", dtype=mytype)

# Convert millisecond time to seconds
time_sec = result["millistime"] / 1000

# Create spline interpolations for T_Water and T_average_of_Al_Block
tck_T_Water = make_interp_spline(time_sec, result["T_Water"])
tck_T_average = make_interp_spline(time_sec, result["T_average_of_Al_Block"])

# Define new time values for smoother plotting
new_time = linspace(time_sec.min(), time_sec.max(), 1000)

# Plot spline fits for T_Water and T_average_of_Al_Block
plot(new_time, tck_T_Water(new_time), label="T_Water")
plot(new_time, tck_T_average(new_time), label="T_average_of_Al_Block")

# Add labels and legend
xlabel('Time [s]')
ylabel('Temperature [C]')
title('Temperature Readings vs Time')
legend()

# Show plot
show()
from pylab import *
from scipy.interpolate import UnivariateSpline
from numpy import dtype

# Define data type and read data
mytype = dtype([("T_Water", "<f4"), ("T_average_of_Al_Block", "<f4"), ("millistime", "<u4"),("heater_state","<u4")])
result = fromfile("Temp_Rec.DAT", dtype=mytype)

# Convert millisecond time to seconds
time_sec = result["millistime"] / 1000

# Create spline interpolations for T_Water and T_average_of_Al_Block
tck_T_Water = UnivariateSpline(time_sec, result["T_Water"])
tck_T_average = UnivariateSpline(time_sec, result["T_average_of_Al_Block"])

# Define new time values for smoother plotting
new_time = linspace(time_sec.min(), time_sec.max(), 1000)

# Create a figure with 3 subplots
fig, axs = subplots(4, 1, figsize=(10, 10))

# Set common x and y limits for all subplots


# Plot spline fits for T_Water and T_average_of_Al_Block
axs[0].plot(new_time, tck_T_Water(new_time), color='red', label="T_Water")
axs[1].plot(new_time, tck_T_average(new_time), color='blue', label="T_average_of_Al_Block")

# Plot temperature readings vs time
axs[2].plot(result["millistime"] / 1000, result["T_Water"], "--", color='red', label="T_Water")
axs[2].plot(result["millistime"] / 1000, result["T_average_of_Al_Block"], "--", color='blue', label="T_average_of_Al_Block")
axs[3].plot(result["millistime"]/1000,result["heater_state"],"o" , color='green', label = "Heater State 0 = off, 1 = on")
# Add labels and legend
axs[0].set_ylabel('Temperature [C]')
axs[1].set_ylabel('Temperature [C]')
axs[0].set_xlabel("Time [s]")
axs[1].set_xlabel("Time [s]")

axs[2].set_xlabel('Time [s]')
axs[2].set_ylabel('Temperature [C]')
axs[0].set_title('Spline fits of temperature vs time')
axs[2].set_title('Temperature readings vs time')
axs[3].set_title("Heater state vs Time")
axs[3].set_xlabel( "Time [s]")
axs[3].set_ylabel("On or Off")

axs[0].legend()
axs[1].legend()
axs[2].legend()
axs[3].legend()

# Adjust spacing between subplots
fig.subplots_adjust(hspace=0.5)

# Show plot
show()
import numpy as np
import control

filename = "DATAREC_5-5-2023.dat"

mytype = np.dtype([
    ("millistime", "<u4"),
    ("Voltage_analog_input", "<f4"),
    ("digi_pot_wiper_position", "<u4"),
    ("T_Water", "<f4"),
    ("T_average_of_Al_Block", "<f4"),
    ("pump_speed_setting", "<u4"),
    ("temp_baseline", "<f4"),
    ("heater_state", "<u4"),
    ("microtime", "<u4"),
])

result = np.fromfile(filename, dtype=mytype)

# To cut out repetitive noise
# Shift millis to start at 0
result["millistime"] = result["millistime"] - result["millistime"][0]
# Create a new numpy array with columns 0, 4, and 7
PID_array = np.vstack(
    (result["millistime"], result["T_average_of_Al_Block"], result["heater_state"]))
# Print the new array
print(PID_array)
sys = control.tf(1, [1, 0])  # Use unity transfer function



# Compute the transfer function of the system using System Identification
# Use the PID tuning method that matches the control algorithm used in your system



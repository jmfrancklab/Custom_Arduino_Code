from pylab import *

filename = "DATAREC.dat"

mytype = dtype([("millistime","<u4"), 
                ("Voltage_analog_input","<f4"),  
                ("digi_pot_wiper_position","<u4"),    
                ("T_Water", "<f4"),
                ("T_average_of_Al_Block", "<f4"),
                ("pump_speed_setting", "<u4"),
                ("temp_baseline","<f4"),
                ("heater_state", "<u4"),
                ("microtime", "<u4")])

result = fromfile(filename, dtype=mytype)
result["millistime"] -= result["millistime"][0]
result["microtime"] -= result["microtime"][0]

fig, axs = subplots(2, 3, figsize=(18, 8))

axs[0, 0].plot(result["millistime"], result["digi_pot_wiper_position"],"--", label="PWO")
axs[0, 0].set_xlabel("Time [mms]")
axs[0, 0].set_ylabel("PWO [0-100]")
axs[0, 0].set_title("Time vs. PWO Position")
axs[0, 0].legend()

axs[0, 1].plot(result["millistime"], result["Voltage_analog_input"],"--", label="Analog")
axs[0, 1].set_xlabel("Time [mms]")
axs[0, 1].set_ylabel("Analog [0-1024]")
axs[0, 1].set_title("Analog Integer Reading vs. Time")
axs[0, 1].legend()

axs[0, 2].plot(result["millistime"],result["heater_state"],"--", label="Heater")
axs[0, 2].set_xlabel("Time [mms]")
axs[0, 2].set_ylabel("Heater on/off")
axs[0, 2].set_title("Heater State vs Time")
axs[0, 2].legend()

axs[1, 0].plot(result["millistime"], result["T_average_of_Al_Block"], "--", label="Average")
axs[1, 0].plot(result["millistime"], result["T_Water"], "--", label="Water")
axs[1, 0].plot(result["millistime"],result["temp_baseline"], label="Baseline")
axs[1, 0].set_xlabel('Time [mms]')
axs[1, 0].set_ylabel('Temperature [C]')
axs[1, 0].set_title('Temp vs. Time (avg)')
axs[1, 0].legend()

axs[1, 1].plot(result["millistime"], result["pump_speed_setting"], "--", label="Pump Speed")
axs[1, 1].set_xlabel("Time [mms]")
axs[1, 1].set_ylabel("pump speed 0-255")
axs[1, 1].set_title("Time vs. Pump Speed")
axs[1, 1].legend()

axs[1, 2].plot(result["microtime"], result["Voltage_analog_input"] ,"--")
axs[1, 2].set_xlabel("Time [us]")
axs[1, 2].set_ylabel("Voltage analog Input")
axs[1, 2].set_title("Fine Time vs Voltage Analog Input")
axs[1, 2].legend()





fig.tight_layout()
show()
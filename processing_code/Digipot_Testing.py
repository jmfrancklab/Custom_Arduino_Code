from pylab import *

filename = "DATAREC.dat"

mytype = dtype([
    ("millistime", "<u4"),
    ("Voltage_analog_input", "<f4"),
    ("digi_pot_wiper_position", "<u4"),
    ("T_Water", "<f4"),
    ("T_average_of_Al_Block", "<f4"),
    ("pump_speed_setting", "<u4"),
    ("temp_baseline", "<f4"),
    ("heater_state", "<u4"),
    ("microtime","<u4"),
    ("DigiV","<u4"),
])

result = fromfile(filename, dtype=mytype)

# Shift millis to start at 0
result["millistime"] = result["millistime"] - result["millistime"][0]

fig, axs = subplots(1, 2, figsize=(18, 8))

axs[0].plot(5000 - 5000/(result["digi_pot_wiper_position"]), (result["Voltage_analog_input"])*5/1024, ".", label="IRLED Voltage")
axs[0].plot(5000 - 5000/(result["digi_pot_wiper_position"]), (result["DigiV"])*5/1024, ".", label="Digi Voltage")
axs[0].set_xlabel("Resistence OHMS of Digi POT")
axs[0].set_ylabel("Voltage")
axs[0].set_title("Response of Voltage Const T and Stepping DigiPot")
axs[0].legend()

axs[1].plot(5000 - 5000/(result["digi_pot_wiper_position"]), (result["T_average_of_Al_Block"]), ".")
axs[1].set_xlabel("Resistence of DigiPot")
axs[1].set_ylabel("Metal Temperature")
axs[1].set_title("Metal Temperature Response")
axs[1].legend()

show()
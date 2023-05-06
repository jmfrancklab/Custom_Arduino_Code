from pylab import *

filename = "INF_TEST.DAT"

mytype = dtype([
  ("millistime","<u4"),
  ("microtime","<u4"),
  ("Voltage_analog_input", "<f4"),
  ("digi_pot_wiper_position","<u4"),
  ("digiVoltageDirect","<u4")
])

result = fromfile(filename, dtype=mytype)

fig, axs = subplots(1, 2, figsize=(18, 8))

axs[0, 0].plot(result["millistime"], result["digi_pot_wiper_position"], "-", label="PWO")
axs[0, 0].set_xlabel("Time [ms]")
axs[0, 0].set_ylabel("PWO [0-100]")
axs[0, 0].set_title("Time vs. PWO Position")
axs[0, 0].legend()

axs[0, 1].plot(result["millistime"], result["Voltage_analog_input"], ".", label="Analog",markersize = 2)
axs[0, 1].set_xlabel("Time [ms]")
axs[0, 1].set_ylabel("Analog [0-1024]")
axs[0, 1].set_title("Analog Integer Reading vs. Time")
axs[0, 1].legend()



show()
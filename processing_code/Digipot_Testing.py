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

print(result)
plot(result["digi_pot_wiper_position"], (result["Voltage_analog_input"])*5/1024, ".", label="Hooked up LED")
plot(result["digi_pot_wiper_position"],(result["digiVoltageDirect"])*5/1024,".",label = "V out From R_AB")
xlabel("Resistence Step 0 -255")
ylabel("Voltage")
title("Analog Integer Reading vs. Step Position of (MCP4131 502E/P )")
legend()



show()
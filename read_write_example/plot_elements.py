from pylab import *



mytype = dtype([    ("millistime","<u4"),    ("microtime","<u4"),    ("Voltage_analog_input","<u4"),    ("digi_pot_wiper_position","<u4")])

result = fromfile("DATATEST.DAT", dtype=mytype)

plot(result["digi_pot_wiper_position"], result["Voltage_analog_input"], "o")

xlabel('PW0 Position [0-100]')
ylabel('Voltage Analog Int [0-1024]')
title('PW0 Position vs. Time')


show()
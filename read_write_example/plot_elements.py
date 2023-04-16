from pylab import *



mytype = dtype([    ("millistime","<u4"),    ("microtime","<u4"),    ("Voltage_analog_input","<f4"),    ("digi_pot_wiper_position","<u4")])

result = fromfile("INF_test.DAT", dtype=mytype)

plot(result["digi_pot_wiper_position"], result["Voltage_analog_input"], "o")

xlabel('PW0 Position [0-100]')
ylabel('Voltage Analog Int [0-1024]')
title('PW0 Position vs. Analog Reading')
figure()

plot(result["millistime"], result["digi_pot_wiper_position"],"o")
xlabel("Time [mms]")
ylabel("PWO [0-100]")
title("Time vs. PWO Position")

figure()

plot(result["millistime"], result["Voltage_analog_input"],"o")
title("Analog Integer Reading vs. Time")
xlabel("Time [mms]")
ylabel("Analog [0-1024]")

figure()
plot(result["microtime"], result["Voltage_analog_input"], "o")

xlabel('Microtime')
ylabel('Voltage Analog Int [0-1024]')
title('Microtime vs. Analog Reading')



show()
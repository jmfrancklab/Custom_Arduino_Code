from pylab import *



filename = "DATAREC.dat"

mytype = dtype([("millistime","<u4"), 
                ("Voltage_analog_input","<u4"),  
                ("digi_pot_wiper_position","<u4"),    
                ("T_Water", "f4"),
                ("T_average_of_Al_Block", "f4"),
                ("pump_speed_setting", "u4"),
                ("temp_baseline","f4"),
                ("heater_state", "u4")])

result = fromfile(filename, dtype=mytype)

plot(result["millistime"], result["Voltage_analog_input"], "o")

xlabel('Time [mms]')
ylabel('Voltage Analog Int [0-1024]')
title('Voltage Analog Reading vs Time')
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
plot(result["millistime"], result["T_average_of_Al_Block"], "o")

xlabel('Time [mms]')
ylabel('Temperature [C]')
title('Temp vs. Time (avg)')



show()
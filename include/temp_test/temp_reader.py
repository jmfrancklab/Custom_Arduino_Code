from pylab import *



mytype = dtype([    ("T_1","<f4"),    ("T_2","<f4"),    ("Average_of_Ts","<f4"),    ("millistime","<u4")])

result = fromfile("Temp_Rec.DAT", dtype=mytype)

plot(result["millistime"], result["T_1"], "o")

xlabel('Time [mms]')
ylabel('Temperature 1 [C]')
title('Temperature 1 Reading vs Time')
figure()

plot(result["millistime"], result["T_2"],"o")
xlabel("Time [mms]")
ylabel("Temperature 2 [C]")
title("Temperature 2 Reading vs Time")

figure()

plot(result["millistime"], result["Average_of_Ts"],"o")
title("Average Reading Temperature vs Time")
xlabel("Time [mms]")
ylabel("Average Temp [C]")




show()
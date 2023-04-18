from pylab import *



mytype = dtype([    ("T_Water","<f4"),    ("T_average_of_Al_Block","<f4"),  ("millistime","<u4")])

result = fromfile("Temp_Rec.DAT", dtype=mytype)

plot(result["millistime"], result["T_Water"], "o")

xlabel('Time [mms]')
ylabel('Temperature of Water [C]')
title('Temperature 1 Reading vs Time')
figure()

plot(result["millistime"], result["T_average_of_Al_Block"],"o")
xlabel("Time [mms]")
ylabel(" Metal Temperature [C]")
title("Metal Temperature Reading vs Time")





show()
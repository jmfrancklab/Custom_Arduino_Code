from pylab import *

mytype = dtype([    ("voltage","<f4"),    ("time","<u4"),    ("finetime","<u4"),    ("Temp_C","<f4")])

result = fromfile("TEST.DAT", dtype=mytype)

plot(result["time"]/60000, result["Temp_C"], "o")

xlabel('Relative Time Difference')
ylabel('Relative Temperature Difference')
title('Temperature vs Time')

text(0.02, 0.95, 'This is not a Unit Converted Relation\nBut the Raw Data Found From A Digital Temp Recorder', 
     transform=plt.gca().transAxes, va='top', ha='left', bbox=dict(facecolor='lightgray', alpha=0.5, pad=5))

show()
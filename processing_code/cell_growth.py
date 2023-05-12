from pylab import *
from scipy.interpolate import UnivariateSpline

# because we wrote the data as a structure, it's natural to read with a numpy
# "structured array" -- this is like an array, but it has fields that can have
# different data types
#
# Think of a structured array as something like a spreadsheet, with different
# columns, where each column can have its own data type.
# I use this because it's exactly designed to read structured data like the
# data given in the arduino example.
mytype = dtype(
        # these are a field name, followed by a string that explains a data type
        # for the string types see here: https://numpy.org/doc/stable/reference/arrays.interface.html#arrays-interface
        # the numbers/sizes here are in bits -- these are taken from the
        # print sizeof command in the example
        [
            ("voltage","<f4"), # search "arduino unsigned int" for the size here
            # garbage accounts for the fact that the overall is not equal
            # to the sum of the parts as explained here:
            # https://stackoverflow.com/questions/32440081/why-is-the-sizeof-of-a-structure-larger-than-the-sum-of-its-parts
            # ("garbage","<u2"),
            # BUT arduino gives sizeof as 6, so there is no garbage 
            ("time","<u4"), # search "arduino long int size" for the size here
            ("finetime","<u4"), # search "arduino long int size" for the size here
            ]
        )
result = fromfile("Cell_Growth.DAT", dtype=mytype)
t = double(result["finetime"])
dt = diff(t)
for idx in nonzero(dt<-1*2**31)[0]:
    currval = t[idx+1] # close to zero or else resulting from previous calls
    t[idx+1:] += result['time'][idx+1]*1e3 - currval
t /= 1e9
V = result["voltage"]*5/1023.
plot(t, V, ".", alpha=1e-2)
# {{{ remove garbage
t = t[:-1000]
V = V[:-1000]
# }}}
avg_over = 10
V = V[:len(V)-len(t)%avg_over]
t = t[:len(t)-len(t)%avg_over]
V = V.reshape(-1,avg_over).mean(axis=1)
t = t.reshape(-1,avg_over).mean(axis=1)
# the following seems to work semi-decently for a couple different versions of avg_over
spl = UnivariateSpline(t, V, s=len(t)*0.5e-2/sqrt(avg_over)) # number of datapoints time variance
plot(t, spl(t))
ylabel('detector reading / V')
xlabel('time / s')
figure()
title('growth rate')
plot(t, -spl.derivative()(t))
show()

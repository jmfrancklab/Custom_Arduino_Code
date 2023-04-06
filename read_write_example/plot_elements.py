from pylab import *

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
            ("Temp_C","<f4")
            ]
        )
result = fromfile("TEST.DAT", dtype=mytype)

#print(result["voltage"][0:20],result["time"][0:20])
plot(result["time"]/60000, result["Temp_C"], "o-")
xlabel("time / ms")
ylabel("temperature / °C")
figure()
plot(result["time"], result["voltage"], "o-")
xlabel("time / ms")
ylabel("voltage / V")
show()

from pylab import *

result = fromfile("file.bin", dtype=float)
print(result)
plot(result, "o-")
show()

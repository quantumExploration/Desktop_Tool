import sys
import numpy as np
import matplotlib.pylab as plt
for x in sys.argv:
	print "Argument: ", x
file_name=sys.argv[1]
print "Read file: "+file_name
data=np.loadtxt(file_name,delimiter=" ",skiprows=1)

	
plt.plot(data[:,0],data[:,1],'ko')
plt.show()
print len(sys.argv)
if len(sys.argv)>2.5:
	file_name=sys.argv[2]
	data=np.loadtxt(file_name,delimiter=" ",skiprows=1)
	n=len(data[:,0])
	for i in range(0,n,2):
		plt.plot(data[i:i+2,0],data[i:i+2,1],'k')
	plt.show()


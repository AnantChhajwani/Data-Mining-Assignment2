import sys
import numpy as np
import matplotlib.pyplot as plt
import subprocess
from subprocess import Popen, PIPE

cut = [1, 5, 10, 25, 50, 90]
method = ['apriori', 'fptree']
time = [[], []]

for i in range(len(cut)):
	for j in range(len(method)):
		cmd = ['./' + method[j], sys.argv[1], method[j] + str(cut[i]) + '.txt', str(cut[i])]
		result = subprocess.Popen(cmd, stdout=subprocess.PIPE)
		out = result.stdout.read()
		time[j].append(float(out.decode('utf-8')))

cut = np.array(cut);
for i in range(len(time)):
	time[i] = np.array(time[i])

plt.plot(cut, time[0], '-o', markersize = 5, label = 'Apriori')
plt.plot(cut, time[1], '-o', markersize = 5, label = 'FP-Tree')
plt.xlabel('Support Threshold(%)')
plt.ylabel('Running Time(s)')
plt.legend()
plt.show()
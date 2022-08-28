import numpy as np
import matplotlib.pyplot as plt

training, setting, threads, time, stddev, vTime, vSetting, vThreads, vTimeSpeedup, vStdDev, vStdDevSpeedup = [], [], [], [], [], [], [], [], [], [], []

for line in open('execLog.txt', 'r'):
 	values = [float(s) for s in line.split()]
 	training.append(int(values[0]))
 	setting.append(int(values[1]))
 	threads.append(int(values[2]))
 	time.append((values[3]))
 	stddev.append((values[4]))

barWidth=0.15

#print (training)
#print (setting)
#print (threads)
#print (time)

countTraining=1;countSetting=1;countThreads=1;

#array sizes w no repeat
for i in range (0, len(training)):
	if training[i] > training[i-1]:
		countTraining += 1 #4

for i in range (0, int(len(setting)/countTraining)):
	if setting[i] > setting[i-1]:
		countSetting += 1 #7

for i in range (0, int(len(threads)/(countTraining*countSetting))):
	if threads[i] > threads[i-1]:
		countThreads += 1 #6

#print (countTraining)
#print (countSetting)
#print (countThreads)

#each batch size
lenghtTraining=int(len(training)/(countTraining))
lenghtSetting=int(lenghtTraining/countSetting)
lenghtThreads=int(lenghtSetting/countThreads)

#print (lenghtTraining) #42
#print (lenghtSetting) #6
#print (lenghtThreads) #1


for i in range (0, countTraining): #4
	fig = plt.figure(figsize=(20,10))
	fig = plt.gcf()
	for j in range (0, countSetting): #7
		for k in range (0, countThreads): #6
			vThreads.append (threads[k+j*lenghtSetting+i*lenghtTraining])
			vTime.append (time[k+j*lenghtSetting+i*lenghtTraining])
			vStdDev.append (stddev[k+j*lenghtSetting+i*lenghtTraining])
			if vTime[k]!=0:
				vTimeSpeedup.append (vTime[0]/vTime[k])
				vStdDevSpeedup.append (vStdDev[0]/vTime[k])
			else:
				vTimeSpeedup.append (vTime[k])
				vStdDevSpeedup.append (vStdDev[k])

		#print (vThreads)
		#print (vTime)
		#print (vTimeSpeedup)

		r4 = np.arange(len(vThreads))
		r1 = [x + barWidth for x in r4]
		r6 = [x + barWidth for x in r1]
		r3 = [x + barWidth for x in r6]
		r2 = [x + barWidth for x in r3]
		r5 = [x + barWidth for x in r2]
		#r7 = [x + barWidth for x in r5]

		if j==0:
			plt.bar(r1, vTimeSpeedup, width=barWidth, edgecolor='black', label = "list")
			plt.errorbar(r1, vTimeSpeedup, vStdDevSpeedup, marker='^', linestyle='None', color='black')
			plt.xticks(r1, vThreads, rotation ='horizontal')

		if j==1:
			plt.bar(r2, vTimeSpeedup, width=barWidth, edgecolor='black', label = "listavx")
			plt.errorbar(r2, vTimeSpeedup, vStdDevSpeedup, marker='^', linestyle='None', color='black')
			plt.xticks(r2, vThreads, rotation ='horizontal')

		if j==2:
			plt.bar(r3, vTimeSpeedup, width=barWidth, edgecolor='black', label = "listsimd")
			plt.errorbar(r3, vTimeSpeedup, vStdDevSpeedup, marker='^', linestyle='None', color='black')
			plt.xticks(r3, vThreads, rotation ='horizontal')
			
		if j==3:
			plt.bar(r4, vTimeSpeedup, width=barWidth, edgecolor='black', label = "parallel")
			plt.errorbar(r4, vTimeSpeedup, vStdDevSpeedup, marker='^', linestyle='None', color='black')
			plt.xticks(r4, vThreads, rotation ='horizontal')

		if j==4:
			plt.bar(r5, vTimeSpeedup, width=barWidth, edgecolor='black', label = "paravx")
			plt.errorbar(r5, vTimeSpeedup, vStdDevSpeedup, marker='^', linestyle='None', color='black')
			plt.xticks(r5, vThreads, rotation ='horizontal')	

		if j==5:
			plt.bar(r6, vTimeSpeedup, width=barWidth, edgecolor='black', label = "parsimd")
			plt.errorbar(r6, vTimeSpeedup, vStdDevSpeedup, marker='^', linestyle='None', color='black')
			plt.xticks(r6, vThreads, rotation ='horizontal')

		#if j==6:
		#	plt.bar(r7, vTimeSpeedup, width=barWidth, edgecolor='black', label = "parseq")
		#	plt.errorbar(r7, vTimeSpeedup, vStdDevSpeedup, marker='^', linestyle='None', color='black')
		#	plt.xticks(r7, vThreads, rotation ='horizontal')

		vSetting.clear()
		vThreads.clear()
		vTime.clear()
		vTimeSpeedup.clear()
		vStdDev.clear()
		vStdDevSpeedup.clear()

	plt.ylabel('Speedup')
	plt.xlabel('Número de threads')

	if i==0:
		filename = "letter"
	if i==1:
		filename = "miniboone" 
	if i==2:
		filename = "SDD" 
	if i==3:
		filename = "SkinSeg" 
	if i==4:
		filename = "statlog" 
	if i==5:
		filename = "susy" 

	handles, labels = plt.gca().get_legend_handles_labels()
	order = [3,0,5,2,1,4]

	plt.title("POPF " + filename + " Speedup Benchmarks")
	plt.legend([handles[idx] for idx in order],[labels[idx] for idx in order])
	plt.grid()
	plt.draw()

	fig.savefig('GraphsSpeedup/'+"graphSpeedup"+filename+".png")
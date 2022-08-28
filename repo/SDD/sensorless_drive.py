n = 58509
attrs = 48
classes = 11

fi = open('Sensorless_drive_diagnosis.txt')
fo = open('sdd.opf.txt', 'w')

fo.write('%d %d %d\n' % (n, classes, attrs))

for i in range(n):
    line = fi.readline().split()
    label = int(line[-1])
    fo.write('%d %d %s\n' % (i, label, ' '.join(line[:-1])))

fi.close()
fo.close()

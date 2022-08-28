#!/usr/bin/python

fi = open('SUSY.csv')
fo = open('SUSY.txt', 'w')

fo.write('5000000 2 18\n')
for i, line in enumerate(fi):
    line = line.strip().split(',')
    line[0] = str(int(float(line[0])) + 1)
    line = [str(i)] + line
    fo.write(' '.join(line) + '\n')

fi.close()
fo.close()

#!/usr/bin/python

fi = open('letter-recognition.data')
fo = open('letter.txt', 'w')

fo.write('20000 26 16\n')
for i, line in enumerate(fi):
    line = line.strip().split(',')
    line[0] = str(int(ord(line[0]) - 64))
    line = [str(i)] + line
    fo.write(' '.join(line) + '\n')

fi.close()
fo.close()

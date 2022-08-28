#!/usr/bin/python

import sys

fi = open(sys.argv[1])
fo = open(sys.argv[2], 'w')

fo.write('%s 7 9\n' % sys.argv[3])
for i, line in enumerate(fi):
    line = line.strip().split(' ')
    line = [str(i), line[-1]] + line[:-1]
    fo.write(' '.join(line) + '\n')

fi.close()
fo.close()

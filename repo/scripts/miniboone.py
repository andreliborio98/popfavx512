#!/usr/bin/python

fi = open('miniboone_orig.txt')
fo = open('miniboone.txt', 'w')

n, m = (int(x) for x in fi.readline().split())

fo.write('%d 2 50\n' % (n + m))

for i in range(n):
    fo.write('%d 1 ' % i)
    fo.write(fi.readline())
    fo.write('\n')
for i in range(m):
    fo.write('%d 2 ' % (i + n))
    fo.write(fi.readline().strip())
    fo.write('\n')

fi.close()
fo.close()

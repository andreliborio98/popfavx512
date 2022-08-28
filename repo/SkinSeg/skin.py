n = 245057
attrs = 3
classes = 2

fi = open('Skin_NonSkin.txt')
fo = open('skin.opf.txt', 'w')

fo.write('%d %d %d\n' % (n, classes, attrs))

for i in range(n):
    line = fi.readline().split()
    label = int(line[-1])
    fo.write('%d %d %s\n' % (i, label, ' '.join(line[:-1])))

fi.close()
fo.close()

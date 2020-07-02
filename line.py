
import os
totalfile = {}
for root, dirs,files in os.walk(os.getcwd(),True):
    flst = [os.path.join(root,f) for f in files if f.endswith(('.c','.h','.cpp'))]
    for fname in flst:
        ft = open(fname)
        totalfile[fname] = len([t for t in ft if len(t.strip())])    
        ft.close()
        print "%s : %d lines" % (fname, totalfile[fname])
print "total %d lines,%d files." % (sum(totalfile.values()), len(totalfile))
os.system('pause')

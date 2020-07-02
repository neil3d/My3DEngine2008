import glob
import sys
import shutil
import os

g_destDir = 'c:/python25/Lib/NexusEngine'
g_sourceDir = 'f:/work/nexus_engine/debug'

def copyByFileExt(sourceDir, ext):
    search = sourceDir+'/*.'+ext    
    fileList = glob.glob(search)
    for f in fileList:
        try:
            print 'copy ', f
            shutil.copy(f, g_destDir)
        except: # may file exist and readonly
            print 'failed!'
    return fileList

def setupMain():    
    try:
        os.makedirs(g_destDir)
    except:
        pass # may dir already exist
        
    copyByFileExt(g_sourceDir, 'dll')
    copyByFileExt(g_sourceDir, 'nex')
    pydFiles = copyByFileExt(g_sourceDir, 'pyd')

    # create __init__.py file
    pkgFileName = g_destDir+'/__init__.py'
    print 'create package file: '+ pkgFileName
    pkgFile = open(pkgFileName, 'w')
    for pyd in pydFiles:
        fileName = os.path.basename(pyd)
        mdlName = os.path.splitext(fileName)[0]
        pkgFile.write('from '+mdlName+' import*\r\n')        
    pkgFile.close()

if __name__ == '__main__':
    setupMain()
    os.system('pause')

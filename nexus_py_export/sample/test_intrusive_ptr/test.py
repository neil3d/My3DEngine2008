from NexusEngine import *

class my_test(CppBase):
    def doIt(self):
        print 'hello'
        
if __name__ == '__main__':
    print '-- object test -----------------------------'
    obj = my_test()
    obj.doIt()

    print '-- object holder test ----------------------'
    hd = Holder()
    hd.put(obj)
    hd.get().doIt()

    print '-- cpp derived object test -----------------------------'
    d = CppDerived()
    d.doIt()


using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace LinqToCodedom
{
    //public class VarRef<T>
    //{
    //    public T v;

    //    public static implicit operator T(VarRef<T> d)
    //    {
    //        return default(T);
    //    }
    //}

    //public class ParamRef<T>
    //{
    //    public T v;

    //    public static implicit operator T(ParamRef<T> d)
    //    {
    //        return default(T);
    //    }
    //}

    public class SetValueRef<T>
    {
        public T v;

        public static implicit operator T(SetValueRef<T> d)
        {
            return default(T);
        }
    }

    public class MemberRef<T>
    {
        public T v;

        public static implicit operator T(MemberRef<T> d)
        {
            return default(T);
        }
    }
}

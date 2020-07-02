using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.CodeDom;

namespace LinqToCodedom.Extensions
{
    public static class CodeConstructorExtensions
    {
        public static CodeConstructor Base(this CodeConstructor ctor, params CodeExpression[] baseCtorArgs)
        {
            ctor.BaseConstructorArgs.AddRange(baseCtorArgs);

            return ctor;
        }

        public static CodeConstructor Base(this CodeConstructor ctor)
        {
            ctor.BaseConstructorArgs.Add(new CodeSnippetExpression(String.Empty));

            return ctor;
        }

        public static CodeConstructor This(this CodeConstructor ctor, params CodeExpression[] thisCtorArgs)
        {
            ctor.ChainedConstructorArgs.AddRange(thisCtorArgs);

            return ctor;
        }

        public static CodeConstructor This(this CodeConstructor ctor)
        {
            ctor.ChainedConstructorArgs.Add(new CodeSnippetExpression(String.Empty));

            return ctor;
        }
    }
}

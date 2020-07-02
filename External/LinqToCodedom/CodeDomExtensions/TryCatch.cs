using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.CodeDom;

namespace LinqToCodedom.Extensions
{
    public static class CodeTryCatchExtensions
    {
        public static CodeTryCatchFinallyStatement AddCatch(this CodeTryCatchFinallyStatement trycatch, 
            Type exceptionType, string varName, params CodeStatement[] statements)
        {
            trycatch.CatchClauses.Add(new CodeCatchClause(varName, new CodeTypeReference(exceptionType), statements));

            return trycatch;
        }

        public static CodeTryCatchFinallyStatement AddCatch(this CodeTryCatchFinallyStatement trycatch, 
            string exceptionType, string varName, params CodeStatement[] statements)
        {
            trycatch.CatchClauses.Add(new CodeCatchClause(varName, new CodeTypeReference(exceptionType), statements));

            return trycatch;
        }

        public static CodeTryCatchFinallyStatement AddFinally(this CodeTryCatchFinallyStatement trycatch,
            params CodeStatement[] statements)
        {
            trycatch.FinallyStatements.AddRange(statements);

            return trycatch;
        }
    }
}

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.CodeDom;
using System.Linq.Expressions;
using LinqToCodedom.Visitors;

namespace LinqToCodedom.Generator
{
    public static partial class Emit
    {
        public static CodeThrowExceptionStatement @throw(Expression<Action> exp)
        {
            VisitorContext ctx = new VisitorContext();
            ctx.VisitParams(exp.Parameters);
            NewExpression newExp = new QueryVisitor((e) => e is NewExpression).Visit(exp) as NewExpression;

            return new CodeThrowExceptionStatement(new CodeObjectCreateExpression(
                new CodeTypeReference(newExp.Type),
                new CodeExpressionVisitor(ctx).VisitExpressionList(newExp.Arguments).ToArray()
            ));
        }
    }
}

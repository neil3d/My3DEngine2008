using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.CodeDom;
using System.Linq.Expressions;
using LinqToCodedom.Generator;
using LinqToCodedom.CodeDomPatterns;
using System.Reflection;
using System.Collections;

namespace LinqToCodedom.Visitors
{
    public class CodeExpressionVisitor
    {
        private VisitorContext _ctx;

        public CodeExpressionVisitor(VisitorContext ctx)
        {
            _ctx = ctx;
        }

        public CodeExpression Visit(Expression exp)
        {
            //Expression c = new QueryVisitor((e) => e.NodeType == ExpressionType.Constant && e.Type.Name.StartsWith("<>c__DisplayClass")).Visit(exp);
            //if (c != null)
            //{
            //    object v = CodeDom.Eval(exp);
            //    return GetFromPrimitive(v);
            //}
            //else
            //{
            CodeExpression res = _Visit(exp);
            if (res is CodeDom.CodeThisExpression)
                res = new CodeThisReferenceExpression();
            else if (res is CodeDom.CodeBaseExpression)
                res = new CodeBaseReferenceExpression();

            return res;
            //}
        }

        private CodeExpression _Visit(Expression exp)
        {
            if (exp == null)
                return null;

            switch (exp.NodeType)
            {
                case ExpressionType.Negate:
                case ExpressionType.NegateChecked:
                case ExpressionType.Not:
                case ExpressionType.Convert:
                case ExpressionType.ConvertChecked:
                case ExpressionType.ArrayLength:
                case ExpressionType.Quote:
                case ExpressionType.TypeAs:
                    return this.VisitUnary((UnaryExpression)exp);
                case ExpressionType.Add:
                case ExpressionType.AddChecked:
                case ExpressionType.Subtract:
                case ExpressionType.SubtractChecked:
                case ExpressionType.Multiply:
                case ExpressionType.MultiplyChecked:
                case ExpressionType.Divide:
                case ExpressionType.Modulo:
                case ExpressionType.And:
                case ExpressionType.AndAlso:
                case ExpressionType.Or:
                case ExpressionType.OrElse:
                case ExpressionType.LessThan:
                case ExpressionType.LessThanOrEqual:
                case ExpressionType.GreaterThan:
                case ExpressionType.GreaterThanOrEqual:
                case ExpressionType.Equal:
                case ExpressionType.NotEqual:
                case ExpressionType.Coalesce:
                case ExpressionType.ArrayIndex:
                case ExpressionType.RightShift:
                case ExpressionType.LeftShift:
                case ExpressionType.ExclusiveOr:
                    return this.VisitBinary((BinaryExpression)exp);
                case ExpressionType.TypeIs:
                    return this.VisitTypeIs((TypeBinaryExpression)exp);
                case ExpressionType.Conditional:
                    return this.VisitConditional((ConditionalExpression)exp);
                case ExpressionType.Constant:
                    return this.VisitConstant((ConstantExpression)exp);
                case ExpressionType.Parameter:
                    return _ctx.VisitParameter((ParameterExpression)exp);
                case ExpressionType.MemberAccess:
                    return this.VisitMemberAccess((MemberExpression)exp);
                case ExpressionType.Call:
                    return this.VisitMethodCall((MethodCallExpression)exp);
                case ExpressionType.Lambda:
                    return this.VisitLambda((LambdaExpression)exp);
                case ExpressionType.New:
                    return this.VisitNew((NewExpression)exp);
                case ExpressionType.NewArrayInit:
                case ExpressionType.NewArrayBounds:
                    return this.VisitNewArray((NewArrayExpression)exp);
                case ExpressionType.Invoke:
                    return this.VisitInvocation((InvocationExpression)exp);
                case ExpressionType.MemberInit:
                //return this.VisitMemberInit((MemberInitExpression)exp);
                case ExpressionType.ListInit:
                //return this.VisitListInit((ListInitExpression)exp);
                default:
                    throw new NotImplementedException(string.Format("Unhandled expression type: '{0}'", exp.NodeType));
            }
        }

        private CodeExpression VisitTypeIs(TypeBinaryExpression typeBinaryExpression)
        {
            var c = _Visit(typeBinaryExpression.Expression);
            return new CodeIsExpression(
                new CodeTypeReference(typeBinaryExpression.TypeOperand), c
            );
        }

        private CodeExpression VisitConditional(ConditionalExpression conditionalExpression)
        {
            return new CodeTernaryExpression(
                _Visit(conditionalExpression.Test),
                _Visit(conditionalExpression.IfTrue),
                _Visit(conditionalExpression.IfFalse)
            );
        }

        private CodeExpression VisitNew(NewExpression newExpression)
        {
            return new CodeObjectCreateExpression(
                new CodeTypeReference(newExpression.Type),
                VisitExpressionList(newExpression.Arguments).ToArray()
            );
        }

        private CodeExpression VisitInvocation(InvocationExpression invocationExpression)
        {
            CodeExpression to = _Visit(invocationExpression.Expression);

            if (typeof(CodeMethodInvokeExpression).IsAssignableFrom(to.GetType()))
            {
                CodeMethodInvokeExpression mi = to as CodeMethodInvokeExpression;
                if (invocationExpression.Arguments.Count > 0)
                {
                    if (invocationExpression.Arguments[0] is ConstantExpression)
                        mi.Parameters.Add(_Visit(invocationExpression.Arguments[0]));
                    else
                        foreach (var par in (invocationExpression.Arguments[0] as NewArrayExpression).Expressions)
                        {
                            AddParam(mi.Parameters, par);
                        }
                }
                return mi;
            }
            else if (to is CodeDelegateInvokeExpression)
            {
                if (invocationExpression.Arguments.Count > 0)
                {
                    if (invocationExpression.Arguments[0] is ConstantExpression)
                        (to as CodeDelegateInvokeExpression).Parameters.Add(_Visit(invocationExpression.Arguments[0]));
                    else
                        foreach (var par in (invocationExpression.Arguments[0] as NewArrayExpression).Expressions)
                        {
                            AddParam((to as CodeDelegateInvokeExpression).Parameters, par);
                        }
                }
                return to;
            }
            else
            {
                var mi = new CodeDelegateInvokeExpression(to);
                if (invocationExpression.Arguments.Count > 0)
                {
                    if (invocationExpression.Arguments[0] is ConstantExpression)
                        mi.Parameters.Add(_Visit(invocationExpression.Arguments[0]));
                    else
                        foreach (var par in invocationExpression.Arguments)
                        {
                            AddParam(mi.Parameters, par);
                        }
                }
                return mi;
            }

        }

        //public CodeExpressionCollection VisitExpressionList(System.Collections.ObjectModel.ReadOnlyCollection<Expression> original)
        public CodeExpressionCollection VisitExpressionList(IEnumerable<Expression> original)
        {
            CodeExpressionCollection list = new CodeExpressionCollection();
            foreach (Expression e in original)
            {
                list.Add(_Visit(e));
            }
            return list;
        }

        public CodeExpressionCollection VisitArguments(IEnumerable<Expression> original)
        {
            CodeExpressionCollection list = new CodeExpressionCollection();
            foreach (Expression e in original)
            {
                AddParam(list, e);
            }
            return list;
        }

        private CodeExpressionCollection VisitSequence(LambdaExpression lambda)
        {
            var me = lambda.Body as MethodCallExpression;
            if (me == null)
                throw new NotSupportedException();

            return VisitArguments((me.Arguments[0] as NewArrayExpression).Expressions);
        }

        private CodeExpression VisitNewArray(NewArrayExpression newArrayExpression)
        {
            Type t = newArrayExpression.Type;//.GetElementType();
            CodeTypeReference tr = new CodeTypeReference(t);
            CodeArrayCreateExpression arr = new CodeArrayCreateExpression(tr);
            //if (t.GetElementType().IsArray)
            //    arr = new CodeArrayCreateExpression(new CodeTypeReference(tr,1));

            if (newArrayExpression.NodeType == ExpressionType.NewArrayBounds)
            {
                if (newArrayExpression.Expressions.Count > 1)
                {
                    return new CodeMultidimensionalArrayCreateExpression(tr, 
                        VisitExpressionList(newArrayExpression.Expressions));
                }
                else
                {
                    arr.SizeExpression = _Visit(newArrayExpression.Expressions[0]);
                }
            }
            else if (newArrayExpression.NodeType == ExpressionType.NewArrayInit)
            {
                foreach (CodeExpression exp in VisitExpressionList(newArrayExpression.Expressions))
                {
                    arr.Initializers.Add(exp);
                }
            }
            else
                throw new NotSupportedException();

            return arr;
        }

        private CodeExpression VisitUnary(UnaryExpression unaryExpression)
        {
            var c = _Visit(unaryExpression.Operand);
            switch (unaryExpression.NodeType)
            {
                case ExpressionType.Convert:
                    if (unaryExpression.Type != typeof(Object))
                    {
                        if (unaryExpression.Method == null)
                            return new CodeCastExpression(unaryExpression.Type, c);
                        else
                        {
                            Type dt = unaryExpression.Method.DeclaringType;
                            if (dt == typeof(Var) ||
                                (dt.IsGenericType &&
                                /*(dt.GetGenericTypeDefinition() == typeof(VarRef<>)) ||
                                (dt.GetGenericTypeDefinition() == typeof(ParamRef<>)) ||*/
                                    (dt.GetGenericTypeDefinition() == typeof(MemberRef<>))
                                )
                            )
                                return c;
                            else
                                return new CodeCastExpression(unaryExpression.Type, c);
                        }
                    }
                    if (c != null)
                        return c;
                    break;
                case ExpressionType.Quote:
                    if (c != null)
                        return c;
                    break;
                case ExpressionType.TypeAs:
                    return new CodeAsExpression(new CodeTypeReference(unaryExpression.Type), c);
                case ExpressionType.Not:
                    return new CodeNotExpression(_Visit(unaryExpression.Operand));
            }
            throw new NotImplementedException(unaryExpression.NodeType.ToString());
        }

        public static CodeMethodReferenceExpression GetMethodRef(System.Reflection.MethodInfo methodInfo)
        {
            var c = new CodeMethodReferenceExpression()
            {
                MethodName = methodInfo.Name
            };
            if (methodInfo.IsStatic)
                c.MethodName = methodInfo.DeclaringType.FullName + "." + methodInfo.Name;
            return c;
        }

        private CodeExpression VisitMethodCall(MethodCallExpression methodCallExpression)
        {
            var mr = GetMethodRef(methodCallExpression.Method);
            if (methodCallExpression.Object == null)
            {
                if (mr.MethodName == "LinqToCodedom.Generator.CodeDom.VarRef" ||
                    mr.MethodName == "LinqToCodedom.Generator.CodeDom.ParamRef")
                {
                    return new LinqToCodedom.Generator.CodeDom.CodeVarExpression(
                        CodeDom.Eval<string>(methodCallExpression.Arguments[0]));
                }

                else if (mr.MethodName == "LinqToCodedom.Generator.CodeDom.TypeRef")
                {
                    var c = new CodeTypeReferenceExpression(
                        CodeDom.Eval<string>(methodCallExpression.Arguments[0]));

                    if (methodCallExpression.Arguments.Count == 2)
                    {
                        NewArrayExpression arr = methodCallExpression.Arguments[1] as NewArrayExpression;
                        foreach (Expression ee in arr.Expressions)
                        {
                            object t = CodeDom.Eval(ee);
                            c.Type.TypeArguments.Add(CodeDom.GetTypeReference(t));
                        }

                    }
                    return c;
                }
                else if (mr.MethodName == "LinqToCodedom.Generator.CodeDom.TypeOf")
                {
                    var c = new CodeTypeOfExpression(
                        CodeDom.Eval<string>(methodCallExpression.Arguments[0]));

                    if (methodCallExpression.Arguments.Count == 2)
                    {
                        NewArrayExpression arr = methodCallExpression.Arguments[1] as NewArrayExpression;
                        foreach (Expression ee in arr.Expressions)
                        {
                            object t = CodeDom.Eval(ee);
                            c.Type.TypeArguments.Add(CodeDom.GetTypeReference(t));
                        }
                    }

                    return c;
                }
                else if (mr.MethodName == "LinqToCodedom.Generator.CodeDom.get_nil")
                {
                    return null;
                }
                else if (mr.MethodName == "LinqToCodedom.Generator.CodeDom.Property")
                {
                    CodeExpression targetExp = _Visit(methodCallExpression.Arguments[0]);
                    if (targetExp is CodePrimitiveExpression && ((CodePrimitiveExpression)targetExp).Value == null)
                    {
                        targetExp = null;
                    }

                    return new CodePropertyReferenceExpression(
                        targetExp,
                        CodeDom.Eval<string>(methodCallExpression.Arguments[1]));
                }
                else if (mr.MethodName == "LinqToCodedom.Generator.CodeDom.Field")
                {
                    object val = null;
                    try
                    {
                        val = CodeDom.Eval(methodCallExpression.Arguments[0]);
                    }
                    catch (Exception)
                    {
                    }

                    if (val != null && val is CodeTypeReference)
                    {
                        return new CodeFieldReferenceExpression(
                            new CodeTypeReferenceExpression((CodeTypeReference)val),
                            CodeDom.Eval<string>(methodCallExpression.Arguments[1]));
                    }
                    else
                    {
                        CodeExpression targetExp = _Visit(methodCallExpression.Arguments[0]);
                        if (targetExp is CodePrimitiveExpression && ((CodePrimitiveExpression)targetExp).Value == null)
                        {
                            targetExp = null;
                        }

                        return new CodeFieldReferenceExpression(
                            targetExp,
                            CodeDom.Eval<string>(methodCallExpression.Arguments[1]));
                    }
                }
                else if (mr.MethodName == "LinqToCodedom.Generator.CodeDom.Call")
                {
                    if (methodCallExpression.Arguments.Count == 1)
                    {
                        return new CodeMethodInvokeExpression(
                            null,
                            CodeDom.Eval<string>(methodCallExpression.Arguments[0]));
                    }
                    else
                    {
                        CodeExpression targetExp = _Visit(methodCallExpression.Arguments[0]);
                        if (targetExp is CodePrimitiveExpression && ((CodePrimitiveExpression)targetExp).Value == null)
                        {
                            targetExp = null;
                        }

                        var m = new CodeMethodReferenceExpression(
                            targetExp,
                            CodeDom.Eval<string>(methodCallExpression.Arguments[1]));

                        if (methodCallExpression.Arguments.Count == 3)
                        {
                            foreach (Expression e in (methodCallExpression.Arguments[2] as NewArrayExpression).Expressions)
                            {
                                m.TypeArguments.Add(CodeDom.GetTypeReference(CodeDom.Eval(e)));
                            }
                        }

                        return new CodeMethodInvokeExpression(m);
                    }
                }
                else if (mr.MethodName == "LinqToCodedom.Generator.CodeDom.new")
                {
                    object t = CodeDom.Eval(methodCallExpression.Arguments[0]);
                    CodeTypeReference type = CodeDom.GetTypeReference(t);

                    if (methodCallExpression.Arguments.Count == 2)
                    {
                        NewArrayExpression arr = methodCallExpression.Arguments[1] as NewArrayExpression;
                        var exp = new CodeObjectCreateExpression(type);
                        foreach (Expression par in arr.Expressions)
                        {
                            AddParam(exp.Parameters, par);
                        }
                        return exp;
                    }
                    else
                        return new CodeObjectCreateExpression(type);
                }
                else if (mr.MethodName == "LinqToCodedom.Generator.CodeDom.cast")
                {
                    object t = CodeDom.Eval(methodCallExpression.Arguments[0]);
                    CodeTypeReference type = CodeDom.GetTypeReference(t);

                    if (methodCallExpression.Method.IsGenericMethod && methodCallExpression.Method.GetGenericArguments()[0] == typeof(Var))
                    {
                        return new CodeDom.CodeWrapExpression(new CodeCastExpression(type, _Visit(methodCallExpression.Arguments[1])));
                    }
                    else
                        return new CodeCastExpression(type, _Visit(methodCallExpression.Arguments[1]));
                }
                else if (mr.MethodName == "LinqToCodedom.Generator.CodeDom.default")
                {
                    object t = CodeDom.Eval(methodCallExpression.Arguments[0]);
                    CodeTypeReference type = CodeDom.GetTypeReference(t);
                    return new CodeDefaultValueExpression(type);
                }
                else if (mr.MethodName == "LinqToCodedom.Generator.CodeDom.InjectExp")
                {
                    int num = CodeDom.Eval<int>(methodCallExpression.Arguments[0]);
                    return _ctx.Injections[num];
                }
                else if (mr.MethodName == "LinqToCodedom.Generator.CodeDom.Is")
                {
                    return new CodeIdentityEqualityExpression(
                        true,
                        _Visit(methodCallExpression.Arguments[0]),
                        _Visit(methodCallExpression.Arguments[1])
                    );
                }
                else if (mr.MethodName == "LinqToCodedom.Generator.CodeDom.IsNot")
                {
                    return new CodeIdentityEqualityExpression(
                        false,
                        _Visit(methodCallExpression.Arguments[0]),
                        _Visit(methodCallExpression.Arguments[1])
                    );
                }
                else if (mr.MethodName == "LinqToCodedom.Generator.CodeDom.Lambda")
                {
                    if (methodCallExpression.Arguments[0].Type.IsArray)
                    {
                        List<LambdaParam> pars = new List<LambdaParam>();
                        foreach (LambdaParam lambdaParam in CodeDom.Eval<IEnumerable>(methodCallExpression.Arguments[0]))
                        {
                            pars.Add(lambdaParam);
                        }
                        List<CodeStatement> stmts = new List<CodeStatement>();
                        foreach (CodeStatement stmt in CodeDom.Eval<IEnumerable>(methodCallExpression.Arguments[1]))
                        {
                            stmts.Add(stmt);
                        }
                        return new CodeLambdaStatements(stmts, pars);
                    }
                    else
                    {
                        CodeExpression exp = _Visit(methodCallExpression.Arguments[0]);
                        List<LambdaParam> pars = new List<LambdaParam>();
                        if (methodCallExpression.Arguments.Count == 2)
                        {
                            NewArrayExpression arr = methodCallExpression.Arguments[1] as NewArrayExpression;
                            foreach (Expression par in arr.Expressions)
                            {
                                pars.Add(CodeDom.Eval<LambdaParam>(par));
                            }
                        }
                        return new CodeLambdaExpression(exp, pars);
                    }
                }
                else if (mr.MethodName == "LinqToCodedom.Generator.CodeDom.CallDelegate")
                {
                    CodeExpression target = null;
                    if (methodCallExpression.Arguments[0].Type == typeof(string))
                    {
                        target = new CodeVariableReferenceExpression(CodeDom.Eval<string>(methodCallExpression.Arguments[0]));
                    }
                    else
                    {
                        target = _Visit(methodCallExpression.Arguments[0]);
                    }

                    var d = new CodeDelegateInvokeExpression(target);

                    return d;
                }
            }

            var to = _Visit(methodCallExpression.Object);
            if (to is CodeDom.CodeThisExpression || to is CodeDom.CodeBaseExpression || to is CodeDom.CodeVarExpression || to is CodeDom.CodeWrapExpression)
            {
                CodeExpression rto = to is CodeDom.CodeThisExpression ?
                    new CodeThisReferenceExpression() :
                    to is CodeDom.CodeBaseExpression ?
                        new CodeBaseReferenceExpression() :
                        to is CodeVariableReferenceExpression ?
                        to as CodeVariableReferenceExpression :
                        to;

                switch (mr.MethodName)
                {
                    case "Call":
                    //case "CallFunction":
                        if (methodCallExpression.Arguments.Count > 0)
                        {
                            var m = new CodeMethodReferenceExpression(
                                rto,
                                CodeDom.Eval<string>(methodCallExpression.Arguments[0]));

                            if (methodCallExpression.Arguments.Count == 2)
                            {
                                foreach (Expression e in (methodCallExpression.Arguments[1] as NewArrayExpression).Expressions)
                                {
                                    m.TypeArguments.Add(CodeDom.GetTypeReference(CodeDom.Eval(e)));
                                }
                            }

                            return new CodeMethodInvokeExpression(m);
                        }
                        else
                        {
                            return new CodeDelegateInvokeExpression(rto);
                        }
                    case "Property":
                        string propertyName = CodeDom.Eval<string>(methodCallExpression.Arguments[0]);
                        if (methodCallExpression.Arguments.Count > 1)
                            throw new NotImplementedException();
                        return new CodeDom.CodeWrapExpression(new CodePropertyReferenceExpression(rto, propertyName));
                    case "Field":
                        string fieldName = CodeDom.Eval<string>(methodCallExpression.Arguments[0]);
                        if (methodCallExpression.Arguments.Count > 1)
                            throw new NotImplementedException();
                        return new CodeFieldReferenceExpression(rto, fieldName);
                    case "Raise":
                        string eventName = CodeDom.Eval<string>(methodCallExpression.Arguments[0]);
                        return new CodeDom.CodeDelegateArgsInvoke(
                            new CodeEventReferenceExpression(rto, eventName));
                    case "ArrayGet":
                        return new CodeArrayIndexerExpression(rto,
                            VisitExpressionList((methodCallExpression.Arguments[0] as NewArrayExpression).Expressions).ToArray()
                        );
                    case "JaggedArrayGet":
                        var n = methodCallExpression.Arguments[0] as NewArrayExpression;
                        CodeArrayIndexerExpression prev = null;
                        foreach (CodeExpression e in VisitExpressionList(n.Expressions.Reverse()))
                        {
                            if (prev == null)
                                prev = new CodeArrayIndexerExpression(rto, e);
                            else
                                prev = new CodeArrayIndexerExpression(prev, e);
                        }
                        return prev;
                    case "cast":
                        return rto;
                    default:
                        throw new NotImplementedException(mr.MethodName);
                }
            }
            //else if (to is CodeDom.CodeArgsInvoke)
            //{
            //    var c = to as CodeMethodInvokeExpression;
            //    c.Parameters.AddRange(VisitArguments(methodCallExpression.Arguments));
            //    //foreach (CodeExpression par in VisitSequence(
            //    //    new QueryVisitor((e) => e is LambdaExpression)
            //    //        .Visit(methodCallExpression.Arguments[0]) as LambdaExpression))
            //    //{
            //    //    c.Parameters.Add(par);
            //    //}
            //    return c;
            //}
            //else if (to is CodeDom.CodeDelegateArgsInvoke)
            //{
            //    var c = to as CodeDelegateInvokeExpression;
            //    c.Parameters.AddRange(VisitArguments(methodCallExpression.Arguments));
            //    //foreach (CodeExpression par in VisitSequence(
            //    //    new QueryVisitor((e) => e is LambdaExpression)
            //    //        .Visit(methodCallExpression.Arguments[0]) as LambdaExpression))
            //    //{
            //    //    c.Parameters.Add(par);
            //    //}
            //    return c;
            //}
            else
            {
                if ((methodCallExpression.Object != null && methodCallExpression.Object.Type.IsArray &&
                    mr.MethodName == "Get") ||
                    (mr.MethodName == "get_Item" && methodCallExpression.Method.IsSpecialName))
                {
                    var c = new CodeArrayIndexerExpression();
                    foreach (var par in methodCallExpression.Arguments)
                    {
                        AddParam(c.Indices, par);
                    }
                    c.TargetObject = to;
                    return c;
                }
                else
                {
                    var c = new CodeMethodInvokeExpression(mr);
                    foreach (var par in methodCallExpression.Arguments)
                    {
                        AddParam(c.Parameters, par);
                    }
                    c.Method.TargetObject = to;
                    return c;
                }
            }
        }

        private void AddParam(CodeExpressionCollection @params, Expression par)
        {
            try
            {
                object v = CodeDom.Eval(par);
                if (v == null && par != null)
                    @params.Add(_Visit(par));
                else
                    @params.Add(GetFromPrimitive(v));
            }
            catch (Exception)
            {
                @params.Add(_Visit(par));
            }
        }

        private CodeExpression GetExpression(Expression exp)
        {
            try
            {
                object v = CodeDom.Eval(exp);
                return GetFromPrimitive(v);
            }
            catch (Exception)
            {
                return _Visit(exp);
            }
        }

        private CodeExpression VisitBinary(BinaryExpression binaryExpression)
        {
            CodeBinaryOperatorType operType = default(CodeBinaryOperatorType);

            switch (binaryExpression.NodeType)
            {
                case ExpressionType.ArrayIndex:
                    return new CodeArrayIndexerExpression(
                        GetExpression(binaryExpression.Left), new CodeExpression[] { GetExpression(binaryExpression.Right) }
                    );
                case ExpressionType.Add:
                    operType = CodeBinaryOperatorType.Add;
                    break;
                case ExpressionType.AddChecked:
                    operType = CodeBinaryOperatorType.Add;
                    break;
                case ExpressionType.And:
                    operType = CodeBinaryOperatorType.BitwiseAnd;
                    break;
                case ExpressionType.AndAlso:
                    operType = CodeBinaryOperatorType.BooleanAnd;
                    break;
                case ExpressionType.Divide:
                    operType = CodeBinaryOperatorType.Divide;
                    break;
                case ExpressionType.Equal:
                    operType = CodeBinaryOperatorType.ValueEquality;
                    break;
                case ExpressionType.ExclusiveOr:
                    return new CodeXorExpression(_Visit(binaryExpression.Left), _Visit(binaryExpression.Right));
                case ExpressionType.GreaterThan:
                    operType = CodeBinaryOperatorType.GreaterThan;
                    break;
                case ExpressionType.GreaterThanOrEqual:
                    operType = CodeBinaryOperatorType.GreaterThanOrEqual;
                    break;
                case ExpressionType.LessThan:
                    operType = CodeBinaryOperatorType.LessThan;
                    break;
                case ExpressionType.LessThanOrEqual:
                    operType = CodeBinaryOperatorType.LessThanOrEqual;
                    break;
                case ExpressionType.Modulo:
                    operType = CodeBinaryOperatorType.Modulus;
                    break;
                case ExpressionType.Multiply:
                    operType = CodeBinaryOperatorType.Multiply;
                    break;
                case ExpressionType.MultiplyChecked:
                    operType = CodeBinaryOperatorType.Multiply;
                    break;
                case ExpressionType.NotEqual:
                    operType = CodeBinaryOperatorType.IdentityInequality;
                    break;
                case ExpressionType.Or:
                    operType = CodeBinaryOperatorType.BitwiseOr;
                    break;
                case ExpressionType.OrElse:
                    operType = CodeBinaryOperatorType.BooleanOr;
                    break;
                case ExpressionType.Subtract:
                    operType = CodeBinaryOperatorType.Subtract;
                    break;
                case ExpressionType.SubtractChecked:
                    operType = CodeBinaryOperatorType.Subtract;
                    break;
                default:
                    throw new NotImplementedException(binaryExpression.NodeType.ToString());
            }

            return new CodeBinaryOperatorExpression(
                        _Visit(binaryExpression.Left), operType, _Visit(binaryExpression.Right));
        }

        private CodeExpression VisitLambda(LambdaExpression lambdaExpression)
        {
            _ctx.VisitParams(lambdaExpression.Parameters);

            var c = _Visit(lambdaExpression.Body);

            if (c.GetType() == typeof(CodeDom.CodeNilExpression))
                return _ctx.Params[0];

            return c;
        }

        private CodeExpression VisitConstant(ConstantExpression constantExpression)
        {
            if (constantExpression.Value == null)
                return new CodePrimitiveExpression(null);
            else
            {
                return GetFromPrimitive(constantExpression.Value);
            }
        }

        public static CodeExpression GetFromPrimitive(object v)
        {
            if (v != null)
            {
                Type t = v.GetType();
                if (typeof(CodeExpression).IsAssignableFrom(t))
                {
                    return v as CodeExpression;
                }
                else if (t.IsEnum)
                {
                    if (t.GetCustomAttributes(false).Any(a => a is FlagsAttribute))
                    {
                        string[] values = v.ToString().Split(',');
                        if (values.Length > 1)
                        {
                            CodeBinaryOperatorExpression op = new CodeBinaryOperatorExpression(
                                new CodeFieldReferenceExpression(new CodeTypeReferenceExpression(t), values[0].Trim()),
                                CodeBinaryOperatorType.BitwiseOr,
                                new CodeFieldReferenceExpression(new CodeTypeReferenceExpression(t), values[1].Trim())
                            );
                            foreach (string item in values.Skip(2))
                            {
                                op = new CodeBinaryOperatorExpression(
                                    op,
                                    CodeBinaryOperatorType.BitwiseOr,
                                    new CodeFieldReferenceExpression(new CodeTypeReferenceExpression(t), item.Trim())
                                );
                            }
                            return op;
                        }
                    }
                    return new CodeFieldReferenceExpression(
                        new CodeTypeReferenceExpression(t), v.ToString());
                }
                else if (typeof(Type).IsAssignableFrom(t))
                    return new CodeTypeOfExpression(v as Type);
                else if (typeof(System.Reflection.MemberInfo).IsAssignableFrom(t))
                {
                    System.Reflection.MemberInfo mi = v as System.Reflection.MemberInfo;
                    return new CodePrimitiveExpression(mi.Name);
                }
                else if (typeof(Var) == t)
                {
                    return new CodeVariableReferenceExpression((v as Var).Name);
                }
                else if (typeof(This) == t)
                {
                    return new CodeThisReferenceExpression();
                }
                else if (typeof(Base) == t)
                {
                    return new CodeBaseReferenceExpression();
                }
                else if (typeof(Prop) == t)
                {
                    return new CodePropertyReferenceExpression(GetFromPrimitive((v as Prop).Target), (v as Var).Name);
                }
                else if (typeof(Field) == t)
                {
                    return new CodeFieldReferenceExpression(GetFromPrimitive((v as Field).Target), (v as Var).Name);
                }
                //else if (typeof(CodeExpression).IsAssignableFrom(t))
                //    return v as CodeExpression;
            }

            return new CodePrimitiveExpression(v);
        }

        private CodeExpression VisitMemberAccess(MemberExpression memberExpression)
        {
            if (memberExpression.Expression == null)
            {
                if (memberExpression.Type == typeof(CodeDom.NilClass))
                {
                    if (memberExpression.Member.Name == "nil")
                    {
                        return new CodeDom.CodeNilExpression();
                    }
                    else
                        throw new NotImplementedException();
                }
                else if (typeof(Base).IsAssignableFrom(memberExpression.Type))
                {
                    if (memberExpression.Member.Name == "this")
                        return new CodeDom.CodeThisExpression();
                    else if (memberExpression.Member.Name == "base")
                        return new CodeDom.CodeBaseExpression();
                    else
                        throw new NotImplementedException();
                }
                else
                    return new CodeSnippetExpression(memberExpression.Type.ToString() + "." + memberExpression.Member.Name);
            }
            else
            {
                var c = _Visit(memberExpression.Expression);
                if (c is CodeSnippetExpression)
                    throw new NotImplementedException();
                else if (c is CodePrimitiveExpression)
                {
                    //object v = ((CodePrimitiveExpression)c).Value;
                    //if (v != null && v.GetType().Name.StartsWith("<>c__DisplayClass"))
                    //{
                    return GetFromPrimitive(CodeDom.Eval(memberExpression));
                    //}
                    //else
                    //{
                    //    MethodInfo mi = memberExpression.Member as MethodInfo;
                    //    if (mi != null)
                    //    {
                    //    }
                    //    else
                    //    {
                    //        ProInfo mi = memberExpression.Member as MethodInfo;
                    //    }
                    //}
                }
                else
                {
                    PropertyInfo pi = memberExpression.Member as PropertyInfo;
                    if (pi != null)
                    {
                        c = new CodePropertyReferenceExpression(c, pi.Name);
                    }
                    else
                        throw new NotImplementedException(memberExpression.Member.GetType().ToString());
                }
                
                return c;
            }
        }
    }
}

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Linq.Expressions;
using System.Collections.ObjectModel;

namespace LinqToCodedom.Visitors
{
    public class QueryVisitor
    {
        private Func<Expression, bool> _test;
        private bool _m;
        private List<Expression> _l = new List<Expression>();

        public QueryVisitor(Func<Expression, bool> t)
        {
            _test = t;
        }

        public IEnumerable<Expression> VisitMulti(Expression exp)
        {
            _m = true;
            _Visit(exp);
            return _l;
        }

        public Expression Visit(Expression exp)
        {
            return _Visit(exp);
        }

        private Expression _Visit(Expression exp)
        {
            if (exp == null)
                return null;

            if (_test(exp))
            {
                if (_m)
                    _l.Add(exp);
                else
                    return exp;
            }

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
                    return this.VisitParameter((ParameterExpression)exp);
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
                    return this.VisitMemberInit((MemberInitExpression)exp);
                case ExpressionType.ListInit:
                    return this.VisitListInit((ListInitExpression)exp);
                default:
                    throw new Exception(string.Format("Unhandled expression type: '{0}'", exp.NodeType));
            }
        }

        private Expression VisitBinding(MemberBinding binding)
        {
            switch (binding.BindingType)
            {
                case MemberBindingType.Assignment:
                    return this.VisitMemberAssignment((MemberAssignment)binding);
                case MemberBindingType.MemberBinding:
                    return this.VisitMemberMemberBinding((MemberMemberBinding)binding);
                case MemberBindingType.ListBinding:
                    return this.VisitMemberListBinding((MemberListBinding)binding);
                default:
                    throw new Exception(string.Format("Unhandled binding type '{0}'", binding.BindingType));
            }
        }

        private Expression VisitElementInitializer(ElementInit initializer)
        {
            foreach (var p in this.VisitExpressionList(initializer.Arguments))
            {
                if (p != null)
                    return p;
            }
            return null;
        }

        private Expression VisitUnary(UnaryExpression u)
        {
            return this._Visit(u.Operand);
        }

        //private CodeBinaryOperatorType BindOperant(ExpressionType e)
        //{
        //    switch (e)
        //    {
        //        case ExpressionType.Add:
        //        case ExpressionType.AddChecked:
        //            return CodeBinaryOperatorType.Add;

        //        case ExpressionType.And:
        //            return CodeBinaryOperatorType.BitwiseAnd;

        //        case ExpressionType.AndAlso:
        //            return CodeBinaryOperatorType.BooleanAnd;

        //        case ExpressionType.Or:
        //            return CodeBinaryOperatorType.BitwiseOr;

        //        case ExpressionType.OrElse:
        //            return CodeBinaryOperatorType.BooleanOr;

        //        case ExpressionType.ExclusiveOr:
        //        case ExpressionType.ArrayIndex:
        //        case ExpressionType.Coalesce:
        //        case ExpressionType.RightShift:
        //        case ExpressionType.LeftShift:
        //            throw new NotSupportedException("no direct equivalent in codedom,so workarounds not implemented");

        //        case ExpressionType.Equal:
        //            return CodeBinaryOperatorType.IdentityEquality;

        //        case ExpressionType.NotEqual:
        //            return CodeBinaryOperatorType.IdentityInequality;

        //        case ExpressionType.GreaterThan:
        //            return CodeBinaryOperatorType.GreaterThan;

        //        case ExpressionType.GreaterThanOrEqual:
        //            return CodeBinaryOperatorType.GreaterThanOrEqual;

        //        case ExpressionType.LessThan:
        //            return CodeBinaryOperatorType.LessThan;

        //        case ExpressionType.LessThanOrEqual:
        //            return CodeBinaryOperatorType.LessThanOrEqual;

        //        case ExpressionType.Multiply:
        //        case ExpressionType.MultiplyChecked:
        //            return CodeBinaryOperatorType.Multiply;

        //        case ExpressionType.Subtract:
        //        case ExpressionType.SubtractChecked:
        //            return CodeBinaryOperatorType.Subtract;

        //        case ExpressionType.Power:
        //        case ExpressionType.Divide:
        //            return CodeBinaryOperatorType.Divide;

        //        case ExpressionType.Modulo:
        //            return CodeBinaryOperatorType.Modulus;

        //        default:
        //            throw new Exception("are you sure you are right?");
        //    }
        //}

        private Expression VisitBinary(BinaryExpression b)
        {
            var left = _Visit(b.Left);

            if (left != null)
                return left;

            return _Visit(b.Right);
        }

        private Expression VisitTypeIs(TypeBinaryExpression b)
        {
            return _Visit(b.Expression);
        }

        private Expression VisitConstant(ConstantExpression c)
        {
            return null;
        }

        private Expression VisitConditional(ConditionalExpression c)
        {
            Expression ifTrue = this._Visit(c.IfTrue);
            if (ifTrue != null)
                return ifTrue;

            return this._Visit(c.IfFalse);
        }

        private Expression VisitParameter(ParameterExpression p)
        {
            return null;
        }

        private Expression VisitMemberAccess(MemberExpression m)
        {
            return this._Visit(m.Expression);
        }

        private Expression VisitMethodCall(MethodCallExpression m)
        {
            Expression obj = this._Visit(m.Object);

            if (obj != null)
                return obj;

            foreach (var par in this.VisitExpressionList(m.Arguments))
            {
                if (par != null)
                    return par;
            }

            return null;
        }

        protected virtual ReadOnlyCollection<Expression> VisitExpressionList(ReadOnlyCollection<Expression> original)
        {
            List<Expression> list = new List<Expression>();
            for (int i = 0, n = original.Count; i < n; i++)
            {
                list.Add(this._Visit(original[i]));
            }
            return list.AsReadOnly();
        }

        private Expression VisitMemberAssignment(MemberAssignment assignment)
        {
            return this._Visit(assignment.Expression);
        }

        private Expression VisitMemberMemberBinding(MemberMemberBinding binding)
        {
            foreach (var p in this.VisitBindingList(binding.Bindings))
            {
                if (p != null)
                    return p;
            }
            return null;
        }

        private Expression VisitMemberListBinding(MemberListBinding binding)
        {
            foreach (var p in this.VisitElementInitializerList(binding.Initializers))
            {
                if (p != null)
                    return p;
            }

            return null;
        }

        private IEnumerable<Expression> VisitBindingList(ReadOnlyCollection<MemberBinding> original)
        {
            List<Expression> list = new List<Expression>();
            for (int i = 0, n = original.Count; i < n; i++)
            {
                Expression b = this.VisitBinding(original[i]);

                list.Add(b);
            }
            return list;
        }

        private IEnumerable<Expression> VisitElementInitializerList(ReadOnlyCollection<ElementInit> original)
        {
            List<Expression> list = new List<Expression>();
            for (int i = 0, n = original.Count; i < n; i++)
            {
                Expression init = this.VisitElementInitializer(original[i]);

                list.Add(init);
            }
            return list;
        }

        private Expression VisitLambda(LambdaExpression lambda)
        {
            return this._Visit(lambda.Body);
        }

        private Expression VisitNew(NewExpression nex)
        {
            foreach (var p in this.VisitExpressionList(nex.Arguments))
            {
                if (p != null)
                    return p;
            }

            return null;
        }

        private Expression VisitMemberInit(MemberInitExpression init)
        {
            Expression n = this.VisitNew(init.NewExpression);
            if (n != null)
                return n;

            foreach (var p in this.VisitBindingList(init.Bindings))
            {
                if (p != null)
                    return p;
            }

            return null;
        }

        private Expression VisitListInit(ListInitExpression init)
        {
            Expression n = this.VisitNew(init.NewExpression);
            if (n != null)
                return n;

            foreach (var p in this.VisitElementInitializerList(init.Initializers))
            {
                if (p != null)
                    return p;
            }

            return null;
        }

        private Expression VisitNewArray(NewArrayExpression na)
        {
            foreach (var p in this.VisitExpressionList(na.Expressions))
            {
                if (p != null)
                    return p;
            }

            return null;
        }

        private Expression VisitInvocation(InvocationExpression iv)
        {
            var expr = this._Visit(iv.Expression);

            if (expr != null)
                return expr;

            foreach (var p in this.VisitExpressionList(iv.Arguments))
            {
                if (p != null)
                    return p;
            }

            return null;

        }
    }
}

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Linq.Expressions;
using System.CodeDom;
using LinqToCodedom.Visitors;

namespace LinqToCodedom.Generator
{
    public static partial class Emit
    {
        public static CodeAssignStatement assignExp<TResult, T>(Expression<Func<TResult, T>> target,
            CodeExpression exp)
        {
            return new CodeAssignStatement(
                new CodeExpressionVisitor(new VisitorContext()).Visit(target),
                exp
            );
        }

        public static CodeAssignStatement assignExp<TResult>(Expression<Func<TResult>> target,
            CodeExpression exp)
        {
            return new CodeAssignStatement(
                new CodeExpressionVisitor(new VisitorContext()).Visit(target),
                exp
            );
        }

        public static CodeAssignStatement assignVar<TResult, T>(string varName,
            Expression<Func<TResult, T>> exp)
        {
            return new CodeAssignStatement(
                new CodeVariableReferenceExpression(varName),
                new CodeExpressionVisitor(new VisitorContext()).Visit(exp));
        }

        public static CodeAssignStatement assignVar<TResult>(string varName,
            Expression<Func<TResult>> exp)
        {
            return new CodeAssignStatement(
                new CodeVariableReferenceExpression(varName),
                new CodeExpressionVisitor(new VisitorContext()).Visit(exp));
        }

        public static CodeAssignStatement assignVar(string varName,
            CodeExpression exp)
        {
            return new CodeAssignStatement(
                new CodeVariableReferenceExpression(varName),
                exp);
        }

        public static CodeAssignStatement assignField<TResult, T>(CodeExpression target, string fieldName,
            Expression<Func<TResult, T>> exp)
        {
            return new CodeAssignStatement(
                new CodeFieldReferenceExpression(target, fieldName),
                new CodeExpressionVisitor(new VisitorContext()).Visit(exp));
        }

        public static CodeAssignStatement assignField<TResult>(CodeExpression target, string fieldName,
            Expression<Func<TResult>> exp)
        {
            return new CodeAssignStatement(
                new CodeFieldReferenceExpression(target, fieldName),
                new CodeExpressionVisitor(new VisitorContext()).Visit(exp));
        }

        public static CodeAssignStatement assignField<TResult>(CodeExpression target, string fieldName,
            CodeExpression exp)
        {
            return new CodeAssignStatement(
                new CodeFieldReferenceExpression(target, fieldName),
                exp);
        }

        public static CodeAssignStatement assignField<TResult, T>(string fieldName,
           Expression<Func<TResult, T>> ex)
        {
            return assignField<TResult, T>(new CodeThisReferenceExpression(), fieldName, ex);
        }

        public static CodeAssignStatement assignField<TResult>(string fieldName,
            Expression<Func<TResult>> exp)
        {
            return assignField<TResult>(new CodeThisReferenceExpression(), fieldName, exp);
        }

        public static CodeAssignStatement assignField<TResult>(string fieldName,
            CodeExpression exp)
        {
            return assignField<TResult>(new CodeThisReferenceExpression(), fieldName, exp);
        }

        public static CodeAssignStatement assignProperty<TResult, T>(CodeExpression target, string propertyName,
            Expression<Func<TResult, T>> ex)
        {
            return new CodeAssignStatement(
                new CodePropertyReferenceExpression(target, propertyName),
                new CodeExpressionVisitor(new VisitorContext()).Visit(ex));
        }

        public static CodeAssignStatement assignProperty<TResult>(CodeExpression target, string propertyName,
            Expression<Func<TResult>> exp)
        {
            return new CodeAssignStatement(
                new CodePropertyReferenceExpression(target, propertyName),
                new CodeExpressionVisitor(new VisitorContext()).Visit(exp));
        }

        public static CodeAssignStatement assignProperty(CodeExpression target, string propertyName,
            CodeExpression exp)
        {
            return new CodeAssignStatement(
                new CodePropertyReferenceExpression(target, propertyName),
                exp);
        }

        public static CodeAssignStatement assignProperty<TResult, T>(string propertyName,
           Expression<Func<TResult, T>> exp)
        {
            return assignProperty<TResult, T>(new CodeThisReferenceExpression(), propertyName, exp);
        }

        public static CodeAssignStatement assignProperty<TResult>(string propertyName,
            Expression<Func<TResult>> exp)
        {
            return assignProperty<TResult>(new CodeThisReferenceExpression(), propertyName, exp);
        }

        //public static CodeAssignStatement assign<TResult, T>(Expression<Func<TResult, LinqToCodedom.Generator.CodeDom.NilClass>> name,
        //    Expression<Func<TResult, T>> stmt)
        //{
        //    return new CodeAssignStatement(
        //        new CodeExpressionVisitor(new VisitorContext()).Visit(name),
        //        new CodeExpressionVisitor(new VisitorContext()).Visit(stmt));
        //}

        public static CodeAssignStatement assignDelegate(string varName, Base target, string methodName)
        {
            return new CodeAssignStatement(
                new CodeVariableReferenceExpression(varName),
                new CodeMethodReferenceExpression(CodeDom.GetTargetObject(target), methodName)
            );
        }

        public static CodeAssignStatement assignDelegate(string varName, string methodName)
        {
            return new CodeAssignStatement(
                new CodeVariableReferenceExpression(varName),
                new CodeMethodReferenceExpression(null, methodName)
            );
        }
    }
}

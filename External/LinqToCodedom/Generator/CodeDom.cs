using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using LinqToCodedom.CodeDomPatterns;
using LinqToCodedom.Visitors;
using System.CodeDom;
using System.Linq.Expressions;
using System.ComponentModel;

namespace LinqToCodedom.Generator
{
    public static partial class CodeDom
    {
        public class NilClass { }

        [EditorBrowsable(EditorBrowsableState.Never)]
        public class CodeNilExpression : CodeExpression { };
        [EditorBrowsable(EditorBrowsableState.Never)]
        public class CodeThisExpression : CodeExpression { };
        [EditorBrowsable(EditorBrowsableState.Never)]
        public class CodeBaseExpression : CodeExpression { };
        [EditorBrowsable(EditorBrowsableState.Never)]
        public class CodeVarExpression : CodeVariableReferenceExpression
        {
            public CodeVarExpression(string name) : base(name) { }
        };
        [EditorBrowsable(EditorBrowsableState.Never)]
        public class CodeWrapExpression : CodeUnaryExpression
        {
            public CodeWrapExpression(CodeExpression exp) : base(exp) { }
        };
        //[EditorBrowsable(EditorBrowsableState.Never)]
        //public class CodePropWrapExpression : CodeUnaryExpression
        //{
        //    public CodePropWrapExpression(CodePropertyReferenceExpression exp) : base(exp) { }
        //};
        [EditorBrowsable(EditorBrowsableState.Never)]
        public class CodeArgsInvoke : CodeMethodInvokeExpression
        {
            public CodeArgsInvoke(CodeExpression target, string methodName)
                : base(target, methodName)
            {
            }
        };

        [EditorBrowsable(EditorBrowsableState.Never)]
        public class CodeDelegateArgsInvoke : CodeDelegateInvokeExpression
        {
            public CodeDelegateArgsInvoke(CodeExpression target)
                : base(target)
            {
            }
        };

        public static NilClass nil
        {
            get { return null; }
        }

        public static This @this
        {
            get { return new This(); }
        }

        public static Base @base
        {
            get { return new Base(); }
        }

        public static void Seq(params object[] args)
        {
        }

        public static T TypedSeq<T>(T v, params object[] args)
        {
            return v;
        }

        public static T NamedSeq<T>(T anonymousTypeDeclaration)
        {
            return default(T);
        }

        public static T VarRef<T>(string name)
        {
            return default(T);
        }

        public static Var VarRef(string name)
        {
            return new Var(name);
        }

        //public static T ParamRef<T>(string name)
        //{
        //    return default(T);
        //}

        internal static T GetMethodName<T>(LambdaExpression exp, CodeParameterDeclarationExpressionCollection pars)
        {
            foreach (var p in exp.Parameters)
            {
                var par = new CodeParameterDeclarationExpression(p.Type, p.Name);
                
                if (typeof(IRefParam).IsAssignableFrom(p.Type))
                    par.Direction = FieldDirection.Ref;
                
                if (typeof(IOutParam).IsAssignableFrom(p.Type))
                    par.Direction = FieldDirection.Out;

                if (typeof(IParamArray).IsAssignableFrom(p.Type))
                    par.CustomAttributes.Add(Define.Attribute(typeof(ParamArrayAttribute)));

                if (p.Type.IsGenericType && (
                    p.Type.GetGenericTypeDefinition() == typeof(RefParam<>) ||
                    p.Type.GetGenericTypeDefinition() == typeof(OutParam<>) ||
                    p.Type.GetGenericTypeDefinition() == typeof(ParamArray<>))
                    )
                {                    
                    Type rt = p.Type.GetGenericArguments()[0];

                    if (p.Type.GetGenericTypeDefinition() == typeof(ParamArray<>) &&
                        !rt.IsArray)
                        par.Type = new CodeTypeReference(rt.ToString() + "[]");
                    else
                        par.Type = new CodeTypeReference(rt);
                }

                //if (typeof(IParamArray).IsAssignableFrom(p.Type))
                //    pars.Add()
                //else
                //    pars.Add(par);
                pars.Add(par);
            }
            return Eval<T>(exp, pars);
        }

        public static CodeStatement[] CombineStmts(params CodeStatement[] stmts)
        {
            return stmts;
        }

        public static T Eval<T>(Expression exp)
        {
            UnaryExpression ue = exp as UnaryExpression;
            if (ue != null)
            {
                return Eval<T>(ue.Operand);
            }
            else
            {
                LambdaExpression le = exp as LambdaExpression;
                if (le != null)
                {
                    return (T)le.Compile().DynamicInvoke();
                }
                else
                {
                    return (T)Expression.Lambda(exp).Compile().DynamicInvoke();
                }
            }
        }

        public static T Eval<T>(LambdaExpression le, CodeParameterDeclarationExpressionCollection pars)
        {
            List<object> o = new List<object>();
            foreach (var e in le.Parameters)
            {
                o.Add(null);
            }

            foreach (var exp in new QueryVisitor((e) => (e is MethodCallExpression &&
                (e as MethodCallExpression).Object is ParameterExpression &&
                typeof(IDynType).IsAssignableFrom(((e as MethodCallExpression).Object as ParameterExpression).Type)))
                .VisitMulti(le))
            {

                if (exp != null)
                {
                    MethodCallExpression mc = (exp as MethodCallExpression);
                    ParameterExpression pe = mc.Object as ParameterExpression;

                    foreach (CodeParameterDeclarationExpression p in pars)
                    {
                        if (pe.Name == p.Name)
                        {
                            object t = Eval(mc.Arguments[0]);
                            p.Type = GetTypeReference(t);
                        }
                    }
                }
            }

            return (T)le.Compile().DynamicInvoke(o.ToArray());
        }

        public static object Eval(Expression exp)
        {
            return Eval<object>(exp);
        }

        public static CodeTypeReference GetTypeReference(object t)
        {
            CodeTypeReference type = t as CodeTypeReference;
            if (type == null)
            {
                if (t is string)
                    type = new CodeTypeReference(t as string);
                else if (t is Type)
                    type = new CodeTypeReference(t as Type);
                else
                    throw new NotSupportedException(t.ToString());
            }
            return type;
        }

        #region Default

        public static CodeDefaultValueExpression @default(CodeTypeReference type)
        {
            return new CodeDefaultValueExpression(type);
        }

        public static CodeDefaultValueExpression @default(string type)
        {
            return new CodeDefaultValueExpression(new CodeTypeReference(type));
        }

        public static CodeDefaultValueExpression @default(Type type)
        {
            return new CodeDefaultValueExpression(new CodeTypeReference(type));
        }

        #endregion

        #region Properties

        //public void Property<T, TResult>(string name, Expression<Func<T, TResult>> paramsExp)
        //{
        //}

        public static T Property<T>(object target, string name)
        {
            return default(T);
        }

        #endregion

        #region Functions

        public static ParamsDelegate<TReturn> Call<TReturn>(object target, string name)
        {
            return default(ParamsDelegate<TReturn>);
        }

        public static ParamsDelegate<TReturn> Call<TReturn>(string name)
        {
            return default(ParamsDelegate<TReturn>);
        }
        #endregion

        #region Routines

        public static ParamsDelegate Call(object target, string name)
        {
            return default(ParamsDelegate);
        }

        public static ParamsDelegate Call(object target, string name, params CodeTypeReference[] types)
        {
            return default(ParamsDelegate);
        }

        public static ParamsDelegate Call(object target, string name, params Type[] types)
        {
            return default(ParamsDelegate);
        }

        public static ParamsDelegate Call(string name)
        {
            return default(ParamsDelegate);
        }

        #endregion

        #region Fields

        public static T Field<T>(object target, string name)
        {
            return default(T);
        }

        public static Field Field(Base target, string name)
        {
            return new Field(target, name);
        }

        public static object Field(object target, string name)
        {
            return null;
        }

        #endregion

        #region New
        public static CodeExpression @new(string type)
        {
            throw new NotSupportedException();
        }

        public static CodeExpression @new(string type, params object[] param)
        {
            throw new NotSupportedException();
        }

        public static CodeExpression @new(Type type)
        {
            throw new NotSupportedException();
        }

        public static CodeExpression @new(Type type, params object[] param)
        {
            throw new NotSupportedException();
        }

        public static CodeExpression @new(CodeTypeReference type)
        {
            throw new NotSupportedException();
        }

        public static CodeExpression @new(CodeTypeReference type, params object[] param)
        {
            throw new NotSupportedException();
        }

        #endregion

        public static Type TypeOf(string type, params string[] types)
        {
            return typeof(string);
        }

        public static Type TypeOf(Type type, params Type[] types)
        {
            return type;
        }

        public static Type TypeOf(Type type, params string[] types)
        {
            return type;
        }

        //public static CodeTypeOfExpression TypeOf(string type, params string[] types)
        //{
        //    return new CodeTypeOfExpression(TypeRef(type, types));
        //}

        public static CodeTypeReference TypeRef(Type type, params CodeTypeReference[] types)
        {
            return TypeRef(new CodeTypeReference(type), types);
        }

        //public static CodeTypeReference TypeRef(string type, params CodeTypeReference[] types)
        //{
        //    return new CodeTypeReference(type, types);
        //}

        public static CodeTypeReference TypeRef(string type, params string[] types)
        {
            return new CodeTypeReference(type,
                types.Select((t) => new CodeTypeReference(t)).ToArray());
        }

        public static CodeTypeReference TypeRef(Type type, params Type[] types)
        {
            var d = new CodeTypeReference(type);
            d.TypeArguments.AddRange(types.Select((t) => new CodeTypeReference(t)).ToArray());
            return d;
        }

        public static CodeTypeReference TypeRef(Type type, params string[] types)
        {
            return TypeRef(new CodeTypeReference(type), types);
        }

        public static CodeTypeReference TypeRef(CodeTypeReference type, params string[] types)
        {
            var d = type;
            d.TypeArguments.AddRange(types.Select((t) => new CodeTypeReference(t)).ToArray());
            return d;
        }

        public static CodeTypeReference TypeRef(CodeTypeReference type, params CodeTypeReference[] types)
        {
            type.TypeArguments.AddRange(types);
            return type;
        }

        public static CodeExpression[] ToArray(this CodeExpressionCollection col)
        {
            CodeExpression[] param = new CodeExpression[col.Count];
            col.CopyTo(param, 0);
            return param;
        }

        public static CodeParameterDeclarationExpression[] ToArray(this CodeParameterDeclarationExpressionCollection col)
        {
            CodeParameterDeclarationExpression[] param = new CodeParameterDeclarationExpression[col.Count];
            col.CopyTo(param, 0);
            return param;
        }

        public static ParamsDelegate<TReturn> CallDelegate<TReturn>(string varName)
        {
            return default(ParamsDelegate<TReturn>);
        }

        public static ParamsDelegate CallDelegate(string varName)
        {
            return default(ParamsDelegate);
        }

        public static ParamsDelegate<TReturn> CallDelegate<TReturn>(object exp)
        {
            return default(ParamsDelegate<TReturn>);
        }

        public static ParamsDelegate CallDelegate(object exp)
        {
            return default(ParamsDelegate);
        }

        public static CodeExpression GetTargetObject(Base o)
        {
            Type t = o.GetType();

            if (t == typeof(This))
                return new CodeThisReferenceExpression();
            else if (t == typeof(Base))
                return new CodeBaseReferenceExpression();
            else if (t == typeof(Var))
            {
                Var v = o as Var;
                return new CodeVariableReferenceExpression(v.Name);
            }
            else
                throw new NotSupportedException();
        }

        #region GetExpression
        public static CodeExpression GetExpression<TResult, T>(Expression<Func<TResult, T>> exp, params CodeExpression[] injections)
        {
            return new CodeExpressionVisitor(new VisitorContext(injections)).Visit(exp);
        }

        public static CodeExpression GetExpression<TResult>(Expression<Func<TResult>> exp, params CodeExpression[] injections)
        {
            return new CodeExpressionVisitor(new VisitorContext(injections)).Visit(exp);
        }

        public static CodeExpression GetExpression<TResult, T, T2>(Expression<Func<TResult, T, T2>> exp, params CodeExpression[] injections)
        {
            return new CodeExpressionVisitor(new VisitorContext(injections)).Visit(exp);
        }

        public static CodeExpression GetExpression(Expression<Action> exp, params CodeExpression[] injections)
        {
            return new CodeExpressionVisitor(new VisitorContext(injections)).Visit(exp);
        }

        public static CodeExpression GetExpression<T>(Expression<Action<T>> exp, params CodeExpression[] injections)
        {
            return new CodeExpressionVisitor(new VisitorContext(injections)).Visit(exp);
        }

        #endregion

        public static object cast(Type type, object o)
        {
            return null;
        }

        public static T cast<T>(Type type, object o)
            where T : Base
        {
            return default(T);
        }

        public static object cast(string type, object o)
        {
            return null;
        }

        public static T cast<T>(string type, object o)
            where T : Base
        {
            return default(T);
        }

        public static object cast(CodeTypeReference type, object o)
        {
            return null;
        }

        public static T cast<T>(CodeTypeReference type, object o)
            where T : Base
        {
            return default(T);
        }

        public static T InjectExp<T>(int num)
        {
            throw new NotImplementedException();
        }

        public static bool Is(object a, object b)
        {
            throw new NotImplementedException();
        }

        public static bool IsNot(object a, object b)
        {
            throw new NotImplementedException();
        }
        
    }
}

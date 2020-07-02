using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.CodeDom;
using LinqToCodedom.CustomCodeDomGeneration;
using LinqToCodedom.CodeDomPatterns;
using System.Collections;
using LinqToCodedom.Extensions;
using System.IO;

namespace LinqToCodedom
{
    public static class CodeDomTreeProcessor
    {
        class Pair<T, T2>
        {
            public T First;
            public T2 Second;
            public Pair(T first, T2 second)
            {
                First = first;
                Second = second;
            }
        }

        public static void ProcessNS(CodeCompileUnit compileUnit, CodeDomGenerator.Language language, IEnumerable<CodeNamespace> namespaces)
        {
            foreach (CodeNamespace ns in namespaces)
            {
                CodeNamespace ns2add = ns;
                for (int j = 0; j < ns.Types.Count; j++)
                {
                    CodeTypeDeclaration c = ns2add.Types[j];
                    ProcessTypeDeclaration(c, language);
                    List<Pair<int, CodeTypeMember>> toReplace = new List<Pair<int, CodeTypeMember>>();
                    for (int i = 0; i < c.Members.Count; i++)
                    {
                        CodeTypeMember m = c.Members[i];
                        CodeTypeMember newMember = ProcessMember(m, language);
                        if (newMember != m)
                            toReplace.Add(new Pair<int, CodeTypeMember>(i, newMember));
                    }
                    if (toReplace.Count > 0)
                    {
                        if (ns2add == ns)
                        {
                            ns2add = ns.Clone() as CodeNamespace;
                            c = ns2add.Types[j];
                        }

                        foreach (Pair<int, CodeTypeMember> p in toReplace)
                        {
                            int idx = p.First;
                            c.Members.RemoveAt(idx);
                            c.Members.Insert(idx, p.Second);
                        }
                    }
                }
                compileUnit.Namespaces.Add(ns2add);
            }
        }

        private static void ProcessTypeDeclaration(CodeTypeDeclaration td, CodeDomGenerator.Language language)
        {
            if (language == CodeDomGenerator.Language.VB)
            {
                if (td.BaseTypes.Count > 0)
                {
                    CodeTypeReference r = td.BaseTypes[0];
                    if (r.UserData.Contains("linq2codedom:interface"))
                    {
                        if ((bool)r.UserData["linq2codedom:interface"])
                        {
                            bool found = false;
                            for(int i = 1; i < td.BaseTypes.Count;i++)
                            {
                                CodeTypeReference item = td.BaseTypes[i];
                                if (!item.UserData.Contains("linq2codedom:interface") ||
                                    !(bool)item.UserData["linq2codedom:interface"])
                                {
                                    found = true;
                                    td.BaseTypes.RemoveAt(i);
                                    td.BaseTypes.Insert(i, item);
                                    break;
                                }
                            }
                            if (!found)
                                td.BaseTypes.Insert(0, new CodeTypeReference(typeof(object)));
                        }
                    }
                }
            }
        }

        private static CodeTypeMember ProcessMember(CodeTypeMember m, LinqToCodedom.CodeDomGenerator.Language language)
        {
            foreach (CodeDirective directive in m.StartDirectives)
            {
                ProcessDirective(directive, language);
            }

            foreach (CodeDirective directive in m.EndDirectives)
            {
                ProcessDirective(directive, language);
            }

            if (typeof(CodeMemberMethod).IsAssignableFrom(m.GetType()))
            {
                foreach (CodeStatement stmt in ((CodeMemberMethod)m).Statements)
                {
                    ProcessStmt(stmt, language);
                }
            }
            else if (typeof(CodeMemberProperty).IsAssignableFrom(m.GetType()))
            {
                foreach (CodeStatement stmt in ((CodeMemberProperty)m).GetStatements)
                {
                    ProcessStmt(stmt, language);
                }

                foreach (CodeStatement stmt in ((CodeMemberProperty)m).SetStatements)
                {
                    ProcessStmt(stmt, language);
                }
            }
            else if (typeof(CodeConstructor).IsAssignableFrom(m.GetType()))
            {
                ProcessExpr(((CodeConstructor)m).BaseConstructorArgs, language);
                ProcessExpr(((CodeConstructor)m).ChainedConstructorArgs, language);
                ProcessStmt(((CodeConstructor)m).Statements, language);
            }
            else if (typeof(CodeMemberField).IsAssignableFrom(m.GetType()))
            {
                ProcessExpr(((CodeMemberField)m).InitExpression, language);
            }
            else if (typeof(CodeTypeDeclaration).IsAssignableFrom(m.GetType()))
            {
                CodeTypeDeclaration td = (CodeTypeDeclaration)m;
                ProcessTypeDeclaration(td, language);
                foreach (CodeTypeMember ms in td.Members)
                {
                    ProcessMember(ms, language);
                }
            }

            ICustomCodeDomObject co = m as ICustomCodeDomObject;
            if (co != null)
                co.GenerateCode(language);

            if (typeof(CodeMemberMethod).IsAssignableFrom(m.GetType()))
                return ProcessMethod(m as CodeMemberMethod, language);

            return m;
        }

        private static void ProcessStmt(CodeStatement stmt, LinqToCodedom.CodeDomGenerator.Language language)
        {
            if (stmt == null) return;

            foreach (CodeDirective directive in stmt.StartDirectives)
            {
                ProcessDirective(directive, language);
            }

            foreach (CodeDirective directive in stmt.EndDirectives)
            {
                ProcessDirective(directive, language);
            }

            if (typeof(CodeAssignStatement).IsAssignableFrom(stmt.GetType()))
            {
                ProcessExpr(((CodeAssignStatement)stmt).Left, language);
                ProcessExpr(((CodeAssignStatement)stmt).Right, language);
            }
            else if (typeof(CodeAttachEventStatement).IsAssignableFrom(stmt.GetType()))
            {
                ProcessExpr(((CodeAttachEventStatement)stmt).Listener, language);
            }
            else if (typeof(CodeCommentStatement).IsAssignableFrom(stmt.GetType()))
            {

            }
            else if (typeof(CodeConditionStatement).IsAssignableFrom(stmt.GetType()))
            {
                ProcessExpr(((CodeConditionStatement)stmt).Condition, language);
                ProcessStmt(((CodeConditionStatement)stmt).TrueStatements, language);
                ProcessStmt(((CodeConditionStatement)stmt).FalseStatements, language);
            }
            else if (typeof(CodeExpressionStatement).IsAssignableFrom(stmt.GetType()))
            {
                ProcessExpr(((CodeExpressionStatement)stmt).Expression, language);
            }
            else if (typeof(CodeGotoStatement).IsAssignableFrom(stmt.GetType()))
            {
            }
            else if (typeof(CodeIterationStatement).IsAssignableFrom(stmt.GetType()))
            {
                ProcessStmt(((CodeIterationStatement)stmt).IncrementStatement, language);
                ProcessStmt(((CodeIterationStatement)stmt).InitStatement, language);
                ProcessStmt(((CodeIterationStatement)stmt).Statements, language);
                ProcessExpr(((CodeIterationStatement)stmt).TestExpression, language);
            }
            else if (typeof(CodeLabeledStatement).IsAssignableFrom(stmt.GetType()))
            {
                ProcessStmt(((CodeLabeledStatement)stmt).Statement, language);
            }
            else if (typeof(CodeMethodReturnStatement).IsAssignableFrom(stmt.GetType()))
            {
                ProcessExpr(((CodeMethodReturnStatement)stmt).Expression, language);
            }
            else if (typeof(CodeRemoveEventStatement).IsAssignableFrom(stmt.GetType()))
            {
                ProcessExpr(((CodeRemoveEventStatement)stmt).Listener, language);
            }
            else if (typeof(CodeThrowExceptionStatement).IsAssignableFrom(stmt.GetType()))
            {
                ProcessExpr(((CodeThrowExceptionStatement)stmt).ToThrow, language);
            }
            else if (typeof(CodeTryCatchFinallyStatement).IsAssignableFrom(stmt.GetType()))
            {
                ProcessStmt(((CodeTryCatchFinallyStatement)stmt).FinallyStatements, language);
                ProcessStmt(((CodeTryCatchFinallyStatement)stmt).TryStatements, language);
                foreach (CodeCatchClause c in ((CodeTryCatchFinallyStatement)stmt).CatchClauses)
                {
                    ProcessStmt(c.Statements, language);
                }
            }
            else if (typeof(CodeVariableDeclarationStatement).IsAssignableFrom(stmt.GetType()))
            {
                ProcessExpr(((CodeVariableDeclarationStatement)stmt).InitExpression, language);
            }
            else if (typeof(CodeUsingStatement).IsAssignableFrom(stmt.GetType()))
            {
                ProcessStmt(((CodeUsingStatement)stmt).Statements, language);
                ProcessExpr(((CodeUsingStatement)stmt).UsingExpression, language);
            }
            else if (typeof(CodeForeachStatement).IsAssignableFrom(stmt.GetType()))
            {
                ProcessStmt(((CodeForeachStatement)stmt).Statements, language);
                ProcessExpr(((CodeForeachStatement)stmt).IterExpression, language);
            }
            else if (typeof(CodeDoStatement).IsAssignableFrom(stmt.GetType()))
            {
                ProcessStmt(((CodeDoStatement)stmt).Statements, language);
                ProcessExpr(((CodeDoStatement)stmt).TestExpression, language);
            }
            else if (typeof(CodeWhileStatement).IsAssignableFrom(stmt.GetType()))
            {
                ProcessStmt(((CodeWhileStatement)stmt).Statements, language);
                ProcessExpr(((CodeWhileStatement)stmt).TestExpression, language);
            }
            else if (typeof(CodeSwitchStatement).IsAssignableFrom(stmt.GetType()))
            {
                ProcessExpr(((CodeSwitchStatement)stmt).SwitchExpression, language);
                foreach (CodeSwitchStatement.CaseBlock cb in ((CodeSwitchStatement)stmt).Cases)
                {
                    ProcessExpr(cb.CaseExpression, language);
                    ProcessStmt(cb.Statements, language);
                }
            }
            else if (typeof(CodeLockStatement).IsAssignableFrom(stmt.GetType()))
            {
                ProcessExpr(((CodeLockStatement)stmt).LockExpression, language);
                ProcessStmt(((CodeLockStatement)stmt).Statements, language);
            }

            ICustomCodeDomObject co = stmt as ICustomCodeDomObject;
            if (co != null)
                co.GenerateCode(language);
        }

        private static void ProcessDirective(CodeDirective directive, CodeDomGenerator.Language language)
        {
            //if (language == CodeDomGenerator.Language.CSharp)
            //{
            //    if (directive is CodeRegionDirective)
            //    {
            //        CodeRegionDirective r = directive as CodeRegionDirective;
            //        if (!r.RegionText.StartsWith("\""))
            //            r.RegionText = "\"" + r.RegionText + "\"";
            //    }
            //}
        }

        private static void ProcessExpr(CodeExpression codeExpression, CodeDomGenerator.Language language)
        {
            if (codeExpression == null) return;

            if (typeof(CodeArgumentReferenceExpression).IsAssignableFrom(codeExpression.GetType()))
            {

            }
            else if (typeof(CodeArrayCreateExpression).IsAssignableFrom(codeExpression.GetType()))
            {
                ProcessExpr(((CodeArrayCreateExpression)codeExpression).Initializers, language);
                ProcessExpr(((CodeArrayCreateExpression)codeExpression).SizeExpression, language);
            }
            else if (typeof(CodeArrayIndexerExpression).IsAssignableFrom(codeExpression.GetType()))
            {
                ProcessExpr(((CodeArrayIndexerExpression)codeExpression).Indices, language);
            }
            else if (typeof(CodeBaseReferenceExpression).IsAssignableFrom(codeExpression.GetType()))
            {

            }
            else if (typeof(CodeBinaryOperatorExpression).IsAssignableFrom(codeExpression.GetType()))
            {
                ProcessExpr(((CodeBinaryOperatorExpression)codeExpression).Left, language);
                ProcessExpr(((CodeBinaryOperatorExpression)codeExpression).Right, language);
            }
            else if (typeof(CodeCastExpression).IsAssignableFrom(codeExpression.GetType()))
            {
                ProcessExpr(((CodeCastExpression)codeExpression).Expression, language);
            }
            else if (typeof(CodeDefaultValueExpression).IsAssignableFrom(codeExpression.GetType()))
            {
            }
            else if (typeof(CodeDelegateCreateExpression).IsAssignableFrom(codeExpression.GetType()))
            {
                ProcessExpr(((CodeDelegateCreateExpression)codeExpression).TargetObject, language);
            }
            else if (typeof(CodeDelegateInvokeExpression).IsAssignableFrom(codeExpression.GetType()))
            {
                ProcessExpr(((CodeDelegateInvokeExpression)codeExpression).Parameters, language);
                ProcessExpr(((CodeDelegateInvokeExpression)codeExpression).TargetObject, language);
            }
            else if (typeof(CodeDirectionExpression).IsAssignableFrom(codeExpression.GetType()))
            {
                ProcessExpr(((CodeDirectionExpression)codeExpression).Expression, language);
            }
            else if (typeof(CodeEventReferenceExpression).IsAssignableFrom(codeExpression.GetType()))
            {
                ProcessExpr(((CodeEventReferenceExpression)codeExpression).TargetObject, language);
            }
            else if (typeof(CodeFieldReferenceExpression).IsAssignableFrom(codeExpression.GetType()))
            {
                ProcessExpr(((CodeFieldReferenceExpression)codeExpression).TargetObject, language);
            }
            else if (typeof(CodeIndexerExpression).IsAssignableFrom(codeExpression.GetType()))
            {
                ProcessExpr(((CodeIndexerExpression)codeExpression).Indices, language);
                ProcessExpr(((CodeIndexerExpression)codeExpression).TargetObject, language);
            }
            else if (typeof(CodeMethodInvokeExpression).IsAssignableFrom(codeExpression.GetType()))
            {
                ProcessExpr(((CodeMethodInvokeExpression)codeExpression).Method, language);
                ProcessExpr(((CodeMethodInvokeExpression)codeExpression).Parameters, language);
            }
            else if (typeof(CodeMethodReferenceExpression).IsAssignableFrom(codeExpression.GetType()))
            {
                ProcessExpr(((CodeMethodReferenceExpression)codeExpression).TargetObject, language);
            }
            else if (typeof(CodeObjectCreateExpression).IsAssignableFrom(codeExpression.GetType()))
            {
                ProcessExpr(((CodeObjectCreateExpression)codeExpression).Parameters, language);
            }
            else if (typeof(CodeParameterDeclarationExpression).IsAssignableFrom(codeExpression.GetType()))
            {
                
            }
            else if (typeof(CodePrimitiveExpression).IsAssignableFrom(codeExpression.GetType()))
            {
            }
            else if (typeof(CodePropertyReferenceExpression).IsAssignableFrom(codeExpression.GetType()))
            {
                ProcessExpr(((CodePropertyReferenceExpression)codeExpression).TargetObject, language);
            }
            else if (typeof(CodePropertySetValueReferenceExpression).IsAssignableFrom(codeExpression.GetType()))
            {

            }
            else if (typeof(CodeThisReferenceExpression).IsAssignableFrom(codeExpression.GetType()))
            {
            }
            else if (typeof(CodeTypeOfExpression).IsAssignableFrom(codeExpression.GetType()))
            {
            }
            else if (typeof(CodeTypeReferenceExpression).IsAssignableFrom(codeExpression.GetType()))
            {
            }
            else if (typeof(CodeVariableReferenceExpression).IsAssignableFrom(codeExpression.GetType()))
            {
            }
            else if (typeof(CodeAssignExpression).IsAssignableFrom(codeExpression.GetType()))
            {
                ProcessExpr(((CodeAssignExpression)codeExpression).Expression, language);
            }
            else if (typeof(CodeAsExpression).IsAssignableFrom(codeExpression.GetType()))
            {
                ProcessExpr(((CodeAsExpression)codeExpression).Expression, language);
            }
            else if (typeof(CodeIsExpression).IsAssignableFrom(codeExpression.GetType()))
            {
                ProcessExpr(((CodeIsExpression)codeExpression).Expression, language);
            }
            else if (typeof(CodeXorExpression).IsAssignableFrom(codeExpression.GetType()))
            {
                ProcessExpr(((CodeXorExpression)codeExpression).Left, language);
                ProcessExpr(((CodeXorExpression)codeExpression).Right, language);
            }
            else if (typeof(CodeUnaryExpression).IsAssignableFrom(codeExpression.GetType()))
            {
                ProcessExpr(((CodeUnaryExpression)codeExpression).Expression, language);
            }

            ICustomCodeDomObject co = codeExpression as ICustomCodeDomObject;
            if (co != null)
                co.GenerateCode(language);
        }

        private static void ProcessExpr(IEnumerable codeExpressions, LinqToCodedom.CodeDomGenerator.Language language)
        {
            foreach (CodeExpression exp in codeExpressions)
                ProcessExpr(exp, language);
        }

        private static void ProcessStmt(IEnumerable stmts, LinqToCodedom.CodeDomGenerator.Language language)
        {
            foreach (CodeStatement stmt in stmts)
                ProcessStmt(stmt, language);
        }

        private static CodeMemberMethod ProcessMethod(CodeMemberMethod method, LinqToCodedom.CodeDomGenerator.Language language)
        {
            if (method.TypeParameters.Count > 0)
            {
                foreach (CodeTypeParameter genericParam in method.TypeParameters)
                {
                    for (int i = 0; i < genericParam.Constraints.Count; i++)
                    {
                        if (genericParam.Constraints[i].BaseType == typeof(object).ToString())
                        {
                            if (language == LinqToCodedom.CodeDomGenerator.Language.CSharp)
                                genericParam.Constraints[i] = new CodeTypeReference(" class");
                            else if (language == LinqToCodedom.CodeDomGenerator.Language.VB)
                                genericParam.Constraints[i] = new CodeTypeReference(" Class");
                        }
                        else if (genericParam.Constraints[i].BaseType == typeof(ValueType).ToString())
                        {
                            if (language == LinqToCodedom.CodeDomGenerator.Language.CSharp)
                                genericParam.Constraints[i] = new CodeTypeReference(" struct");
                            else if (language == LinqToCodedom.CodeDomGenerator.Language.VB)
                                genericParam.Constraints[i] = new CodeTypeReference(" Structure");
                        }
                    }
                }
            }

            if (language == LinqToCodedom.CodeDomGenerator.Language.VB)
            {
                if (method.PrivateImplementationType != null)
                {
                    CodeMemberMethod newMethod = method.Clone() as CodeMemberMethod;
                    newMethod.ImplementationTypes.Add(method.PrivateImplementationType);
                    newMethod.PrivateImplementationType = null;
                    return newMethod;
                }
            }

            //object o = method.UserData["linq2codedom:partial"];
            //if (o != null)
            //{
            //    bool partial = (bool)o;

                //if (partial)
                //{
                //    string typeStr = null;

                //    switch (language)
                //    {
                //        case CodeDomGenerator.Language.CSharp:
                //            using (Microsoft.CSharp.CSharpCodeProvider provider = new Microsoft.CSharp.CSharpCodeProvider())
                //            {
                //                System.CodeDom.Compiler.CodeGeneratorOptions opts = new System.CodeDom.Compiler.CodeGeneratorOptions();
                //                using (System.CodeDom.Compiler.IndentedTextWriter tw = new System.CodeDom.Compiler.IndentedTextWriter(new StringWriter(), opts.IndentString))
                //                {
                //                    provider.(method.ReturnType, tw, opts);
                //                    typeStr = tw.InnerWriter.ToString();
                //                }
                //            }
                //            break;
                //        case CodeDomGenerator.Language.VB:
                //            using (Microsoft.VisualBasic.VBCodeProvider provider = new Microsoft.VisualBasic.VBCodeProvider())
                //            {
                //                System.CodeDom.Compiler.CodeGeneratorOptions opts = new System.CodeDom.Compiler.CodeGeneratorOptions();
                //                using (System.CodeDom.Compiler.IndentedTextWriter tw = new System.CodeDom.Compiler.IndentedTextWriter(new StringWriter(), opts.IndentString))
                //                {
                //                    tw.Write("(");
                //                    provider.GenerateCodeFromExpression(Left, tw, opts);
                //                    tw.Write(" Xor ");
                //                    provider.GenerateCodeFromExpression(Right, tw, opts);
                //                    tw.Write(")");
                //                    Value = tw.InnerWriter.ToString();
                //                }
                //            }
                //            break;
                //        default:
                //            throw new NotImplementedException(language.ToString());
                //    }
                //}
            //}
            return method;
        }
    }
}

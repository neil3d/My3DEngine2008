using System;
using System.CodeDom;
using System.CodeDom.Compiler;
using System.Collections;
using System.IO;
using LinqToCodedom.CustomCodeDomGeneration;
using System.Text;
using Microsoft.CSharp;

namespace LinqToCodedom.CodeDomPatterns
{
    public class CodeCustomEvent : CodeSnippetTypeMember, ICustomCodeDomObject
    {
        public CodeMemberEvent Event { get; set; }
        public CodeMemberProperty AddMethod { get; set; }
        public CodeMemberProperty RemoveMethod { get; set; }
        public CodeMemberMethod RaiseMethod { get; set; }

        public CodeCustomEvent(CodeMemberEvent @event, CodeMemberProperty add, CodeMemberProperty remove)
        {
            Event = @event;
            AddMethod = add;
            RemoveMethod = remove;
        }

        public CodeCustomEvent(CodeMemberEvent @event, CodeMemberProperty add, CodeMemberProperty remove, CodeMemberMethod raise)
            : this(@event, add, remove)
        {
            RaiseMethod = raise;
        }

        public void GenerateCode(CodeDomGenerator.Language language)
        {
            switch (language)
            {
                case CodeDomGenerator.Language.CSharp:
                    using (CSharpCodeProvider provider = new CSharpCodeProvider())
                    {
                        CodeGeneratorOptions opts = new CodeGeneratorOptions();
                        StringBuilder sb = new StringBuilder();
                        using (IndentedTextWriter tw = new IndentedTextWriter(new StringWriter(sb), opts.IndentString))
                        {
                            provider.GenerateCodeFromMember(Event, tw, opts);
                            sb.Replace(";",string.Empty);
                            tw.WriteLine("{");
                            tw.Indent++;
                            GenrateMeth(opts, provider, tw, AddMethod.GetStatements, "add");
                            GenrateMeth(opts, provider, tw, RemoveMethod.GetStatements, "remove");
                            tw.Indent--;
                            tw.WriteLine("}");
                        }
                        Text = sb.ToString();
                    }
                    break;
                case CodeDomGenerator.Language.VB:
                    using (Microsoft.VisualBasic.VBCodeProvider provider = new Microsoft.VisualBasic.VBCodeProvider())
                    {
                        CodeGeneratorOptions opts = new CodeGeneratorOptions();
                        var args = new StringWriter();
                        provider.GenerateCodeFromExpression(new CodeParameterDeclarationExpression(Event.Type, "value"), args, opts);
                        
                        StringBuilder sb = new StringBuilder();
                        using (IndentedTextWriter tw = new IndentedTextWriter(new StringWriter(sb), opts.IndentString))
                        {
                            provider.GenerateCodeFromMember(Event, tw, opts);
                            sb.Replace(" Event ", " Custom Event ");
                            tw.Indent++;

                            tw.WriteLine("AddHandler({0})", args.GetStringBuilder());
                            tw.Indent++;
                            foreach (CodeStatement statement in AddMethod.GetStatements)
                            {
                                provider.GenerateCodeFromStatement(statement, tw, opts);
                            }
                            tw.Indent--;
                            tw.WriteLine("End AddHandler");

                            tw.WriteLine("RemoveHandler({0})", args.GetStringBuilder());
                            tw.Indent++;
                            foreach (CodeStatement statement in RemoveMethod.GetStatements)
                            {
                                provider.GenerateCodeFromStatement(statement, tw, opts);
                            }
                            tw.Indent--;
                            tw.WriteLine("End RemoveHandler");

                            //args = new StringWriter();
                            //provider.GenerateCodeFromMember(RaiseMethod, args, opts);

                            //StringReader sr = new StringReader(args.GetStringBuilder().ToString());
                            //string line = sr.ReadLine();
                            //while (string.IsNullOrEmpty(line))
                            //    line = sr.ReadLine();
                            //int idx = line.IndexOf(" " + RaiseMethod.Name);
                            //args.GetStringBuilder().Replace(line, "RaiseEvent" + line.Substring(idx + RaiseMethod.Name.Length + 1));
                            //tw.Write(args.GetStringBuilder().ToString());

                            tw.Write("RaiseEvent(");
                            for (int i = 0; i < RaiseMethod.Parameters.Count;i++)
                            {
                                if (i > 0)
                                    tw.Write(", ");
                                provider.GenerateCodeFromExpression(RaiseMethod.Parameters[i], tw, opts);
                            }
                            tw.WriteLine(")");
                            tw.Indent++;
                            foreach (CodeStatement statement in RaiseMethod.Statements)
                            {
                                provider.GenerateCodeFromStatement(statement, tw, opts);
                            }
                            tw.Indent--;
                            tw.WriteLine("End RaiseEvent");

                            tw.Indent--;
                            tw.WriteLine("End Event");
                        }
                        Text = sb.ToString();
                    }
                    break;
                default:
                    throw new NotImplementedException(language.ToString());
            }
        }

        private static void GenrateMeth(CodeGeneratorOptions opts, CodeDomProvider provider, 
            IndentedTextWriter tw, CodeStatementCollection statements, string methName)
        {
            tw.WriteLine(methName);
            tw.WriteLine("{");
            tw.Indent++;
            foreach (CodeStatement statement in statements)
            {
                provider.GenerateCodeFromStatement(statement, tw, opts);
            }
            tw.Indent--;
            tw.WriteLine("}");
        }
    }
}

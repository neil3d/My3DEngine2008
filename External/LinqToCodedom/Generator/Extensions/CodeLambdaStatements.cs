using System;
using System.Collections.Generic;
using System.Text;
using System.CodeDom;
using LinqToCodedom.CustomCodeDomGeneration;
using System.IO;
using LinqToCodedom.Generator;

namespace LinqToCodedom.CodeDomPatterns
{
    public class CodeLambdaStatements : CodeSnippetExpression, ICustomCodeDomObject
	{
		private IEnumerable<CodeStatement> _statements;
        private List<LambdaParam> _params;

        public CodeLambdaStatements(IEnumerable<CodeStatement> stmts, List<LambdaParam> lambdaParams)
		{
			_statements = stmts;
            LambdaParams = lambdaParams;
		}

        public IEnumerable<CodeStatement> Statements
		{
			get { return _statements; }
			set
			{
				if (_statements != value)
				{
					_statements = value;
				}
			}
		}

        public List<LambdaParam> LambdaParams
        {
            get { return _params; }
            set { _params = value; }
        }

        public void GenerateCode(CodeDomGenerator.Language language)
        {
            switch (language)
            {
                case CodeDomGenerator.Language.CSharp:
                    using (Microsoft.CSharp.CSharpCodeProvider provider = new Microsoft.CSharp.CSharpCodeProvider())
                    {
                        System.CodeDom.Compiler.CodeGeneratorOptions opts = new System.CodeDom.Compiler.CodeGeneratorOptions();
                        using (System.CodeDom.Compiler.IndentedTextWriter tw = new System.CodeDom.Compiler.IndentedTextWriter(new StringWriter(), opts.IndentString))
                        {
                            if (_params.Count != 1 || _params[0].Type != null)
                                tw.Write("(");

                            for (int i = 0; i < _params.Count; i++)
                            {
                                if (i > 0)
                                    tw.Write(", ");

                                if (_params[i].Type != null)
                                {
                                    provider.GenerateCodeFromExpression(new CodeTypeReferenceExpression(_params[i].Type), tw, opts);
                                    tw.Write(" ");
                                }

                                tw.Write(_params[i].Name);
                            }

                            if (_params.Count != 1 || _params[0].Type != null)
                                tw.Write(")");

                            tw.WriteLine(" => ");
                            tw.WriteLine("{");
                            tw.Indent++;
                            foreach (CodeStatement statement in Statements)
                            {
                                provider.GenerateCodeFromStatement(statement, tw, opts);
                            }
                            tw.Indent--;
                            tw.Write("}");
                            Value = tw.InnerWriter.ToString();
                        }
                    }
                    break;
                case CodeDomGenerator.Language.VB:
                    using (Microsoft.VisualBasic.VBCodeProvider provider = new Microsoft.VisualBasic.VBCodeProvider())
                    {
                        System.CodeDom.Compiler.CodeGeneratorOptions opts = new System.CodeDom.Compiler.CodeGeneratorOptions();
                        using (System.CodeDom.Compiler.IndentedTextWriter tw = new System.CodeDom.Compiler.IndentedTextWriter(new StringWriter(), opts.IndentString))
                        {
                            throw new NotImplementedException();
                            //tw.Write("Function(");

                            //for (int i = 0; i < _params.Count; i++)
                            //{
                            //    if (i > 0)
                            //        tw.Write(", ");

                            //    tw.Write(_params[i].Name);

                            //    if (_params[i].Type != null)
                            //    {
                            //        tw.Write(" As ");
                            //        provider.GenerateCodeFromExpression(new CodeTypeReferenceExpression(_params[i].Type), tw, opts);
                            //    }
                            //}

                            //tw.Write(") ");
                            //provider.GenerateCodeFromExpression(Statements, tw, opts);
                            //Value = tw.InnerWriter.ToString();
                        }
                    }
                    break;
                default:
                    throw new NotImplementedException(language.ToString());
            }
        }
    }
}

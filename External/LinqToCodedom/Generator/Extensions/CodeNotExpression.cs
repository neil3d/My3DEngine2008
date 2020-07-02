using System;
using System.Collections.Generic;
using System.Text;
using System.CodeDom;
using LinqToCodedom.CustomCodeDomGeneration;
using System.IO;

namespace LinqToCodedom.CodeDomPatterns
{
    public class CodeNotExpression : CodeSnippetExpression, ICustomCodeDomObject
	{
		private CodeExpression m_expression;

        public CodeNotExpression(CodeExpression expression)
		{
			m_expression = expression;
		}

		public CodeExpression Expression
		{
			get { return m_expression; }
			set
			{
				if (m_expression != value)
				{
					m_expression = value;
				}
			}
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
                            tw.Write("!");
                            provider.GenerateCodeFromExpression(Expression, tw, opts);
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
                            tw.Write("Not ");
                            provider.GenerateCodeFromExpression(Expression, tw, opts);
                            Value = tw.InnerWriter.ToString();
                        }
                    }
                    break;
                default:
                    throw new NotImplementedException(language.ToString());
            }
        }
    }
}

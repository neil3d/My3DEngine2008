using System;
using System.Collections.Generic;
using System.Text;
using System.CodeDom;
using LinqToCodedom.CustomCodeDomGeneration;
using System.IO;

namespace LinqToCodedom.CodeDomPatterns
{
    public class CodeMultidimensionalArrayCreateExpression : CodeSnippetExpression, ICustomCodeDomObject
	{
		private CodeTypeReference m_typeReference;
		private CodeExpressionCollection m_expression;

        public CodeMultidimensionalArrayCreateExpression(CodeTypeReference type, CodeExpressionCollection expression)
		{
			m_typeReference = type;
			m_expression = expression;
		}

		public CodeTypeReference TypeReference
		{
			get { return m_typeReference; }
			set 
			{
				if (m_typeReference != value)
				{
					m_typeReference = value;
				}
			}
		}

        public CodeExpressionCollection Dimensions
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
                            tw.Write("new ");
                            provider.GenerateCodeFromExpression(new CodeTypeReferenceExpression(TypeReference.ArrayElementType), tw, opts);
                            tw.Write("[");
                            for (int i = 0; i < Dimensions.Count; i++)
                            {
                                if (i > 0)
                                    tw.Write(",");
                                provider.GenerateCodeFromExpression(Dimensions[i], tw, opts);
                            }
                            tw.Write("]");
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
                            tw.Write("(");
                            for (int i = 0; i < Dimensions.Count; i++)
                            {
                                if (i > 0)
                                    tw.Write(",");
                                tw.Write("(");
                                provider.GenerateCodeFromExpression(Dimensions[i], tw, opts);
                                tw.Write(")-1");
                            }
                            tw.Write(") as ");
                            provider.GenerateCodeFromExpression(new CodeTypeReferenceExpression(TypeReference), tw, opts);
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

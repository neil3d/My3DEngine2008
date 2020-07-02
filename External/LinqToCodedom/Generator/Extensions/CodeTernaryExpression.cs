using System;
using System.Collections.Generic;
using System.Text;
using System.CodeDom;
using LinqToCodedom.CustomCodeDomGeneration;
using System.IO;

namespace LinqToCodedom.CodeDomPatterns
{
    public class CodeTernaryExpression : CodeSnippetExpression, ICustomCodeDomObject
	{
		private CodeExpression _condition;
        private CodeExpression _truePart;
        private CodeExpression _falsePart;

        public CodeTernaryExpression(CodeExpression condition, CodeExpression truePart, CodeExpression falsePart)
		{
			_condition = condition;
            _truePart = truePart;
            _falsePart = falsePart;
		}

		public CodeExpression Condition
		{
			get { return _condition; }
			set
			{
                if (_condition != value)
				{
                    _condition = value;
				}
			}
		}

        public CodeExpression TruePart
        {
            get { return _truePart; }
            set
            {
                if (_truePart != value)
                {
                    _truePart = value;
                }
            }
        }

        public CodeExpression FalsePart
        {
            get { return _falsePart; }
            set
            {
                if (_falsePart != value)
                {
                    _falsePart = value;
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
                            provider.GenerateCodeFromExpression(_condition, tw, opts);
                            tw.Write("?");
                            provider.GenerateCodeFromExpression(_truePart, tw, opts);
                            tw.Write(":");
                            provider.GenerateCodeFromExpression(_falsePart, tw, opts);
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
                            tw.Write("If(");
                            provider.GenerateCodeFromExpression(_condition, tw, opts);
                            tw.Write(",");
                            provider.GenerateCodeFromExpression(_truePart, tw, opts);
                            tw.Write(",");
                            provider.GenerateCodeFromExpression(_falsePart, tw, opts);
                            tw.Write(")");
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

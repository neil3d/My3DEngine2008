using System;
using System.Collections.Generic;
using System.Text;
using System.CodeDom;
using LinqToCodedom.CustomCodeDomGeneration;
using System.IO;

namespace LinqToCodedom.CodeDomPatterns
{
    public class CodeIdentityEqualityExpression : CodeSnippetExpression, ICustomCodeDomObject
	{
		private bool _eq;
        private CodeExpression _left;
        private CodeExpression _right;

        public CodeIdentityEqualityExpression(bool equal, CodeExpression left, CodeExpression right)
		{
            _eq = equal;
            _left = left;
            _right = right;
		}

		public bool IsEqual
		{
			get { return _eq; }
			set
			{
                if (_eq != value)
				{
                    _eq = value;
				}
			}
		}

        public CodeExpression Left
        {
            get { return _left; }
            set
            {
                if (_left != value)
                {
                    _left = value;
                }
            }
        }

        public CodeExpression Right
        {
            get { return _right; }
            set
            {
                if (_right != value)
                {
                    _right = value;
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
                            if (!_eq)
                            {
                                tw.Write("!");
                            }
                            tw.Write("ReferenceEquals(");
                            provider.GenerateCodeFromExpression(_left, tw, opts);
                            tw.Write(", ");
                            provider.GenerateCodeFromExpression(_right, tw, opts);
                            tw.Write(")");
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
                            provider.GenerateCodeFromExpression(_left, tw, opts);
                            if (_eq)
                                tw.Write(" Is ");
                            else
                                tw.Write(" IsNot ");
                            provider.GenerateCodeFromExpression(_right, tw, opts);
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

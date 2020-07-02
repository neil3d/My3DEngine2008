using System;
using System.Collections.Generic;
using System.Text;
using System.CodeDom;
using LinqToCodedom.CustomCodeDomGeneration;
using System.IO;

namespace LinqToCodedom.CodeDomPatterns
{
    public class CodeCustomBinaryExpression : CodeSnippetExpression, ICustomCodeDomObject
	{
		private string _separator;
        private CodeExpression _left;
        private CodeExpression _right;

        public CodeCustomBinaryExpression(string separator)
        {
            _separator = separator;
        }

        public CodeCustomBinaryExpression(string separator, CodeExpression left)
        {
            _separator = separator;
            _left = left;
        }

        public CodeCustomBinaryExpression(string separator, CodeExpression left, CodeExpression right)
		{
            _separator = separator;
            _left = left;
            _right = right;
		}

		public string Separator
		{
			get { return _separator; }
			set
			{
                if (_separator != value)
				{
                    _separator = value;
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

        public CodeCustomBinaryExpression Append(CodeExpression exp)
        {
            if (_right != null)
            {
                return new CodeCustomBinaryExpression(Separator, this, exp);
            }
            else
            {
                if (_left == null)
                    _left = exp;
                else
                    _right = exp;

                return this;
            }
        }

        public void GenerateCode(CodeDomGenerator.Language language)
        {
            if (_left == null) return;

            switch (language)
            {
                case CodeDomGenerator.Language.CSharp:
                    using (Microsoft.CSharp.CSharpCodeProvider provider = new Microsoft.CSharp.CSharpCodeProvider())
                    {
                        System.CodeDom.Compiler.CodeGeneratorOptions opts = new System.CodeDom.Compiler.CodeGeneratorOptions();
                        using (System.CodeDom.Compiler.IndentedTextWriter tw = new System.CodeDom.Compiler.IndentedTextWriter(new StringWriter(), opts.IndentString))
                        {
                            provider.GenerateCodeFromExpression(_left, tw, opts);
                            if (_right != null)
                            {
                                tw.Write(_separator);
                                provider.GenerateCodeFromExpression(_right, tw, opts);
                            }
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
                            if (_right != null)
                            {
                                tw.Write(_separator);
                                provider.GenerateCodeFromExpression(_right, tw, opts);
                            }
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

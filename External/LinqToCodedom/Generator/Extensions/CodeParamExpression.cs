using System;
using System.CodeDom;
using LinqToCodedom.CustomCodeDomGeneration;
using System.IO;
using System.Text;

namespace LinqToCodedom.CodeDomPatterns
{
    public class CodeParamExpression : CodeSnippetExpression, ICustomCodeDomObject
	{
        public ParamOption Option { get; set; }

        public enum ParamOption
        {
            Optional,
            ParamArray
        }

		private CodeParameterDeclarationExpression _param;

        public CodeParamExpression(CodeParameterDeclarationExpression param, ParamOption option)
        {
            Option = option;
            _param = param;
        }

        public CodeParameterDeclarationExpression Parameter
		{
			get { return _param; }
			set 
			{
				if (_param != value)
				{
                    _param = value;
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
                            provider.GenerateCodeFromExpression(Parameter, tw, opts);
                            Value = tw.InnerWriter.ToString();
                        }
                    }
                    break;
                case CodeDomGenerator.Language.VB:
                    using (Microsoft.VisualBasic.VBCodeProvider provider = new Microsoft.VisualBasic.VBCodeProvider())
                    {
                        System.CodeDom.Compiler.CodeGeneratorOptions opts = new System.CodeDom.Compiler.CodeGeneratorOptions();
                        StringBuilder sb = new StringBuilder();
                        using (System.CodeDom.Compiler.IndentedTextWriter tw = new System.CodeDom.Compiler.IndentedTextWriter(new StringWriter(sb), opts.IndentString))
                        {
                            provider.GenerateCodeFromExpression(Parameter, tw, opts);
                            switch (Option)
                            {
                                case ParamOption.Optional:
                                    sb.Replace("ByVal " + Parameter.Name, "Optional ByVal " + Parameter.Name);
                                    break;
                                case ParamOption.ParamArray:
                                    sb.Replace("ByVal " + Parameter.Name, "ByVal ParamArray " + Parameter.Name);
                                    break;
                                default:
                                    throw new NotSupportedException(Option.ToString());
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

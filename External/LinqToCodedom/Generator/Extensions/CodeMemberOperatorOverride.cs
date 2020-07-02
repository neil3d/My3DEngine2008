using System;
using System.CodeDom.Compiler;
using System.Collections.Generic;
using System.IO;
using System.Text;
using System.CodeDom;
using LinqToCodedom.CustomCodeDomGeneration;

namespace LinqToCodedom.CodeDomPatterns
{
	public enum OperatorType
	{
		Explicit,
		Implicit
	}

    public class CodeMemberOperatorOverride : CodeSnippetTypeMember, ICustomCodeDomObject
	{
		protected interface IMemberOperatorGenerator
		{
			void GenerateDeclaration(TextWriter tw, CodeDomProvider provider, CodeGeneratorOptions opts, CodeTypeReference returnType, CodeParameterDeclarationExpression[] parameters);
			void GenerateStatemets(TextWriter tw, CodeDomProvider provider, CodeGeneratorOptions opts, CodeStatement[] statements);
		}

        public CodeMemberOperatorOverride(OperatorType type,
            CodeParameterDeclarationExpression[] parameters,
            CodeTypeReference returnType,
            params CodeStatement[] statements)
        {
            m_operator = type;
            m_parameters = parameters;
            m_returnType = returnType;
            m_statements = statements;
        }

		private OperatorType m_operator;
		private CodeParameterDeclarationExpression[] m_parameters;
		private CodeStatement[] m_statements;
		private CodeTypeReference m_returnType;

		public OperatorType Operator
		{
			get
			{
				return m_operator;
			}
			set
			{
				m_operator = value;
			}
		}

		public CodeTypeReference ReturnType
		{
			get
			{
				return m_returnType;
			}
			set
			{
				m_returnType = value;
			}
		}
		public CodeParameterDeclarationExpression[] Parameters
		{
			get
			{
				return m_parameters;
			}
			set
			{
				m_parameters = value;
			}
		}

		public CodeStatement[] Statements
		{
			get
			{
				return m_statements;
			}
			set
			{
				m_statements = value;
			}
		}

        #region ICustomCodeDomObject Members

        public void GenerateCode(CodeDomGenerator.Language language)
        {
            if (m_parameters == null || m_statements == null || m_returnType == null)
                return;

            switch (language)
            {
                case CodeDomGenerator.Language.CSharp:
                    GenerateCS();
                    break;
                case CodeDomGenerator.Language.VB:
                    GenerateVB();
                    break;
                default:
                    throw new NotImplementedException(language.ToString());
            }
        }

        #region "VB Code"

        private void GenerateVB()
        {
            IMemberOperatorGenerator gen = null;
            switch (Operator)
            {
                case OperatorType.Explicit:
                    gen = new ExplicitOperatorGeneratorVB();
                    break;
                case OperatorType.Implicit:
                    gen = new ImplicitOperatorGeneratorVB();
                    break;
                default:
                    throw new NotImplementedException(string.Format("Какая-то несанкционированная xуйня"));
            }

            using (Microsoft.VisualBasic.VBCodeProvider provider = new Microsoft.VisualBasic.VBCodeProvider())
            {
                System.CodeDom.Compiler.CodeGeneratorOptions opts = new CodeGeneratorOptions();
                using (System.CodeDom.Compiler.IndentedTextWriter tw = new IndentedTextWriter(new StringWriter(), opts.IndentString))
                {
                    gen.GenerateDeclaration(tw, provider, opts, ReturnType, Parameters);
                    tw.Indent++;
                    gen.GenerateStatemets(tw, provider, opts, Statements);
                    tw.Indent--;
                    tw.WriteLine("End Operator");
                    Text = tw.InnerWriter.ToString();
                }
            }
        }

        protected abstract class ConversionOperatorGeneratorBaseVB : CodeMemberOperatorOverride.IMemberOperatorGenerator
        {
            #region Implementation of IMemberOperatorGenerator

            protected abstract string OperatorName
            {
                get;
            }

            public void GenerateDeclaration(TextWriter tw, CodeDomProvider provider, CodeGeneratorOptions opts, CodeTypeReference returnType, CodeParameterDeclarationExpression[] parameters)
            {
                tw.Write(string.Format("Public Shared {0} Operator CType", OperatorName));
                tw.Write("(");
                for (int i = 0; i < parameters.Length; i++)
                {
                    var parameter = parameters[i];
                    provider.GenerateCodeFromExpression(parameter, tw, opts);
                    if (i != (parameters.Length - 1))
                        tw.Write(", ");
                }

                tw.Write(") As ");
                provider.GenerateCodeFromExpression(new CodeTypeReferenceExpression(returnType), tw, opts);
                tw.WriteLine();
            }

            public void GenerateStatemets(TextWriter tw, CodeDomProvider provider, CodeGeneratorOptions opts, CodeStatement[] statements)
            {
                foreach (var statement in statements)
                {
                    provider.GenerateCodeFromStatement(statement, tw, opts);
                }
            }

            #endregion
        }

        protected class ExplicitOperatorGeneratorVB : ConversionOperatorGeneratorBaseVB
        {
            private const string c_name = "Narrowing";

            #region Overrides of ConversionOperatorGeneratorBase

            protected override string OperatorName
            {
                get
                {
                    return c_name;
                }
            }

            #endregion
        }

        protected class ImplicitOperatorGeneratorVB : ConversionOperatorGeneratorBaseVB
        {
            private const string c_name = "Widening";

            #region Overrides of ConversionOperatorGeneratorBase

            protected override string OperatorName
            {
                get
                {
                    return c_name;
                }
            }

            #endregion
        }
        #endregion

        #region "CS Code"

        private void GenerateCS()
        {
            IMemberOperatorGenerator gen = null;
            switch (Operator)
            {
                case OperatorType.Explicit:
                    gen = new ExplicitOperatorGeneratorCS();
                    break;
                case OperatorType.Implicit:
                    gen = new ImplicitOperatorGeneratorCS();
                    break;
                default:
                    throw new NotImplementedException(string.Format("Какая-то несанкционированная xуйня"));
            }

            using (Microsoft.CSharp.CSharpCodeProvider provider = new Microsoft.CSharp.CSharpCodeProvider())
            {
                System.CodeDom.Compiler.CodeGeneratorOptions opts = new System.CodeDom.Compiler.CodeGeneratorOptions();
                using (System.CodeDom.Compiler.IndentedTextWriter tw = new System.CodeDom.Compiler.IndentedTextWriter(new StringWriter(), opts.IndentString))
                {
                    gen.GenerateDeclaration(tw, provider, opts, ReturnType, Parameters);
                    tw.WriteLine("{");
                    tw.Indent++;
                    gen.GenerateStatemets(tw, provider, opts, Statements);
                    tw.Indent--;
                    tw.WriteLine("}");
                    Text = tw.InnerWriter.ToString();
                }
            }
        }

        protected abstract class ConversionOperatorGeneratorBaseCS : CodeMemberOperatorOverride.IMemberOperatorGenerator
        {
            #region Implementation of IMemberOperatorGenerator

            protected abstract string OperatorName
            {
                get;
            }

            public void GenerateDeclaration(TextWriter tw, CodeDomProvider provider, CodeGeneratorOptions opts, CodeTypeReference returnType, CodeParameterDeclarationExpression[] parameters)
            {
                tw.Write(string.Format("public static {0} operator ", OperatorName));
                provider.GenerateCodeFromExpression(new CodeTypeReferenceExpression(returnType), tw, opts);
                tw.Write("(");
                for (int i = 0; i < parameters.Length; i++)
                {
                    var parameter = parameters[i];
                    provider.GenerateCodeFromExpression(parameter, tw, opts);
                    if (i != (parameters.Length - 1))
                        tw.Write(", ");
                }

                tw.WriteLine(")");
            }

            public void GenerateStatemets(TextWriter tw, CodeDomProvider provider, CodeGeneratorOptions opts, CodeStatement[] statements)
            {
                foreach (var statement in statements)
                {
                    provider.GenerateCodeFromStatement(statement, tw, opts);
                }
            }

            #endregion
        }

        protected class ExplicitOperatorGeneratorCS : ConversionOperatorGeneratorBaseCS
        {
            private const string c_name = "explicit";

            #region Overrides of ConversionOperatorGeneratorBase

            protected override string OperatorName
            {
                get
                {
                    return c_name;
                }
            }

            #endregion
        }

        protected class ImplicitOperatorGeneratorCS : ConversionOperatorGeneratorBaseCS
        {
            private const string c_name = "implicit";

            #region Overrides of ConversionOperatorGeneratorBase

            protected override string OperatorName
            {
                get
                {
                    return c_name;
                }
            }

            #endregion
        }

#endregion

        #endregion
    }
}

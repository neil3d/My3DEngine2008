using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using System.CodeDom;
using LinqToCodedom.CustomCodeDomGeneration;

namespace LinqToCodedom.CodeDomPatterns
{
    public class CodeLanguageSnippetStatement : CodeSnippetStatement, ICustomCodeDomObject
    {
        private CodeSnippetStatement _csStatement;
        private CodeSnippetStatement _vbStatement;

        public CodeLanguageSnippetStatement(CodeSnippetStatement csStatement, CodeSnippetStatement vbStatement)
        {
            _csStatement = csStatement;
            _vbStatement = vbStatement;
        }

        public CodeSnippetStatement CSStatement
        {
            get { return _csStatement; }
            set
            {
                _csStatement = value;
            }
        }

        public CodeSnippetStatement VBStatement
        {
            get { return _vbStatement; }
            set
            {
                _vbStatement = value;
            }
        }

        #region ICustomCodeDomObject Members

        public void GenerateCode(CodeDomGenerator.Language language)
        {
            switch (language)
            {
                case CodeDomGenerator.Language.CSharp:
                    if (_csStatement != null)
                        using (Microsoft.CSharp.CSharpCodeProvider provider = new Microsoft.CSharp.CSharpCodeProvider())
                        {
                            System.CodeDom.Compiler.CodeGeneratorOptions opts = new System.CodeDom.Compiler.CodeGeneratorOptions();
                            using (System.CodeDom.Compiler.IndentedTextWriter tw = new System.CodeDom.Compiler.IndentedTextWriter(new StringWriter(), opts.IndentString))
                            {
                                provider.GenerateCodeFromStatement(_csStatement, tw, opts);
                                Value = tw.InnerWriter.ToString();
                            }
                        }
                    break;
                case CodeDomGenerator.Language.VB:
                    if (_vbStatement != null)
                        using (Microsoft.VisualBasic.VBCodeProvider provider = new Microsoft.VisualBasic.VBCodeProvider())
                        {
                            System.CodeDom.Compiler.CodeGeneratorOptions opts = new System.CodeDom.Compiler.CodeGeneratorOptions();
                            using (System.CodeDom.Compiler.IndentedTextWriter tw = new System.CodeDom.Compiler.IndentedTextWriter(new StringWriter(), opts.IndentString))
                            {
                                provider.GenerateCodeFromStatement(_vbStatement, tw, opts);
                                Value = tw.InnerWriter.ToString();
                            }
                        }
                    break;
                default:
                    throw new NotImplementedException(language.ToString());
            }
        }

        #endregion
    }
}

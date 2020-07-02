using System;
using System.Collections.Generic;
using System.CodeDom.Compiler;
using System.CodeDom;
using System.IO;

namespace LinqToCodedom.CustomCodeDomGeneration
{
    public interface ICustomCodeDomObject
    {
        void GenerateCode(CodeDomGenerator.Language language);
    }

    class CustomCodeGenerator : ICodeCompiler, ICodeGenerator
        //CodeCompiler
    {
        readonly CodeDomGenerator.Language _lang;
        //CodeDomProvider _provider;

        public CustomCodeGenerator(CodeDomGenerator.Language lang)
        {
            _lang = lang;
        }

        //public CustomCodeGenerator(CodeDomProvider provider)
        //{
        //    _provider = provider;
        //}

        private ICodeGenerator _generator;

        public ICodeGenerator Generator
        {
            get { return _generator; }
            set { _generator = value; }
        }

        private ICodeCompiler _compiler;

        public ICodeCompiler Compiler
        {
            get { return _compiler; }
            set { _compiler = value; }
        }

        #region ICodeGenerator Members

        public string CreateEscapedIdentifier(string value)
        {
            return _generator.CreateEscapedIdentifier(value);
        }

        public string CreateValidIdentifier(string value)
        {
            return _generator.CreateValidIdentifier(value);
        }

        public void GenerateCodeFromCompileUnit(CodeCompileUnit e, TextWriter w, CodeGeneratorOptions o)
        {
            _generator.GenerateCodeFromCompileUnit(e, w, o);
        }

        public void GenerateCodeFromExpression(CodeExpression e, TextWriter w, CodeGeneratorOptions o)
        {
            ICustomCodeDomObject co = e as ICustomCodeDomObject;
            if (co != null)
                co.GenerateCode(GetLang());

            _generator.GenerateCodeFromExpression(e, w, o);
        }

        private CodeDomGenerator.Language GetLang()
        {
            return _lang;
        }

        public void GenerateCodeFromNamespace(CodeNamespace e, TextWriter w, CodeGeneratorOptions o)
        {
            _generator.GenerateCodeFromNamespace(e, w, o);
        }

        public void GenerateCodeFromStatement(CodeStatement e, TextWriter w, CodeGeneratorOptions o)
        {
            ICustomCodeDomObject co = e as ICustomCodeDomObject;
            if (co != null)
                co.GenerateCode(GetLang());

            _generator.GenerateCodeFromStatement(e, w, o);
        }

        public void GenerateCodeFromType(CodeTypeDeclaration e, TextWriter w, CodeGeneratorOptions o)
        {
            _generator.GenerateCodeFromType(e, w, o);
        }

        public string GetTypeOutput(CodeTypeReference type)
        {
            return _generator.GetTypeOutput(type);
        }

        public bool IsValidIdentifier(string value)
        {
            return _generator.IsValidIdentifier(value);
        }

        public bool Supports(GeneratorSupport supports)
        {
            return _generator.Supports(supports);
        }

        public void ValidateIdentifier(string value)
        {
            _generator.ValidateIdentifier(value);
        }

        #endregion

        #region ICodeCompiler Members

        public CompilerResults CompileAssemblyFromDom(CompilerParameters options, System.CodeDom.CodeCompileUnit compilationUnit)
        {
            return _compiler.CompileAssemblyFromDom(options, compilationUnit);
        }

        public CompilerResults CompileAssemblyFromDomBatch(CompilerParameters options, System.CodeDom.CodeCompileUnit[] compilationUnits)
        {
            return _compiler.CompileAssemblyFromDomBatch(options, compilationUnits);
        }

        public CompilerResults CompileAssemblyFromFile(CompilerParameters options, string fileName)
        {
            return _compiler.CompileAssemblyFromFile(options, fileName);
        }

        public CompilerResults CompileAssemblyFromFileBatch(CompilerParameters options, string[] fileNames)
        {
            return _compiler.CompileAssemblyFromFileBatch(options, fileNames);
        }

        public CompilerResults CompileAssemblyFromSource(CompilerParameters options, string source)
        {
            return _compiler.CompileAssemblyFromSource(options, source);
        }

        public CompilerResults CompileAssemblyFromSourceBatch(CompilerParameters options, string[] sources)
        {
            return _compiler.CompileAssemblyFromSourceBatch(options, sources);
        }

        #endregion
    }

    class VBCustomCodeProvider : Microsoft.VisualBasic.VBCodeProvider
    {
        private CustomCodeGenerator _gen = new CustomCodeGenerator(CodeDomGenerator.Language.VB);

        public VBCustomCodeProvider()
        {
        }

        public VBCustomCodeProvider(IDictionary<string, string> providerOptions)
            : base(providerOptions)
        {
        }

        [Obsolete]
        public override ICodeCompiler CreateCompiler()
        {
            if (_gen.Compiler == null)
                _gen.Compiler = base.CreateCompiler();

            return _gen;
        }

        [Obsolete]
        public override ICodeGenerator CreateGenerator()
        {
            if (_gen.Generator == null)
                _gen.Generator = base.CreateGenerator();

            return _gen;
        }
    }

    class CSCustomCodeProvider : Microsoft.CSharp.CSharpCodeProvider
    {
        private CustomCodeGenerator _gen = new CustomCodeGenerator(CodeDomGenerator.Language.CSharp);

        public CSCustomCodeProvider()
        {
        }

        public CSCustomCodeProvider(IDictionary<string, string> providerOptions)
            : base(providerOptions)
        {
        }

        [Obsolete]
        public override ICodeCompiler CreateCompiler()
        {
            if (_gen.Compiler == null)
                _gen.Compiler = base.CreateCompiler();

            return _gen;
        }

        [Obsolete]
        public override ICodeGenerator CreateGenerator()
        {
            if (_gen.Generator == null)
                _gen.Generator = base.CreateGenerator();

            return _gen;
        }
    }
}

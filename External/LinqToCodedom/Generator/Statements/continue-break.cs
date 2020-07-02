using System.Text;
using System.CodeDom;
using LinqToCodedom.Visitors;
using System.Linq.Expressions;
using LinqToCodedom.CodeDomPatterns;

namespace LinqToCodedom.Generator
{
    public static partial class Emit
    {
        public static CodeContinueForStatement continueFor()
        {
            return new CodeContinueForStatement();
        }

        public static CodeContinueDoStatement continueDo()
        {
            return new CodeContinueDoStatement();
        }

        public static CodeExitForStatement exitFor()
        {
            return new CodeExitForStatement();
        }

        public static CodeExitDoStatement exitDo()
        {
            return new CodeExitDoStatement();
        }

        public static CodeExitDoStatement exitWhile()
        {
            return new CodeExitDoStatement();
        }

        public static CodeExitSelectStatement exitSwitch()
        {
            return new CodeExitSelectStatement();
        }
    }
}

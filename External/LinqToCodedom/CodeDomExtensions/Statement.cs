using System.CodeDom;

namespace LinqToCodedom.Extensions
{
    public static class CodeStatementExtensions
    {
        public static T StartRegion<T>(this T stmt, 
            string title)
            where T : CodeStatement
        {
            stmt.StartDirectives.Add(new CodeRegionDirective(CodeRegionMode.Start, title));

            return stmt;
        }

        public static T EndRegion<T>(this T stmt)
            where T : CodeStatement
        {
            stmt.EndDirectives.Add(new CodeRegionDirective(CodeRegionMode.End, string.Empty));

            return stmt;
        }

        public static T Line<T>(this T stmt, string file, int line)
            where T : CodeStatement
        {
            stmt.LinePragma = new CodeLinePragma(file, line);

            return stmt;
        }
    }
}

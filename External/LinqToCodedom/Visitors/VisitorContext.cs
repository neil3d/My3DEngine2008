using System.Collections.Generic;
using System.CodeDom;
using System.Linq.Expressions;

namespace LinqToCodedom.Visitors
{
    public class VisitorContext
    {
        private readonly List<CodeExpression> _params = new List<CodeExpression>();
        private readonly List<CodeExpression> _injections = new List<CodeExpression>();

        public VisitorContext()
        {

        }

        public VisitorContext(params CodeExpression[] injections)
        {
            _injections.AddRange(injections);
        }

        public List<CodeExpression> Params
        {
            get { return _params; }
        }

        public List<CodeExpression> Injections
        {
            get { return _injections; }
        }

        public CodeExpression VisitParameter(ParameterExpression parameterExpression)
        {
            return _params.Find((p) => (p.UserData["name"] as string) == parameterExpression.Name);
        }

        public void VisitParams(System.Collections.ObjectModel.ReadOnlyCollection<ParameterExpression> @params)
        {
            foreach (var p in @params)
            {
                if (p.Type.IsGenericType)
                {
                    /*if (p.Type.GetGenericTypeDefinition() == typeof(ParamRef<>))
                        _params.Add(new CodeArgumentReferenceExpression(p.Name));
                    else if (p.Type.GetGenericTypeDefinition() == typeof(VarRef<>))
                        _params.Add(new CodeVariableReferenceExpression(p.Name));
                    else*/
                    if (p.Type.GetGenericTypeDefinition() == typeof(SetValueRef<>))
                        _params.Add(new CodePropertySetValueReferenceExpression());
                    else
                        _params.Add(new CodeVariableReferenceExpression(p.Name));

                    _params[_params.Count - 1].UserData["name"] = p.Name;
                }
                else if (p.Type == typeof(Var))
                {
                    _params.Add(new LinqToCodedom.Generator.CodeDom.CodeVarExpression(p.Name));
                    _params[_params.Count - 1].UserData["name"] = p.Name;
                }
                else
                {
                    _params.Add(new CodeVariableReferenceExpression(p.Name));
                    _params[_params.Count - 1].UserData["name"] = p.Name;
                }
            }
        }

    }
}

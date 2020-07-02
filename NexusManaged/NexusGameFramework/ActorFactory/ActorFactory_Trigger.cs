using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Nexus.GameFramework
{
    /// <summary>
    /// NTriggerActor Factory
    /// </summary>
    public class ActorFactory_Trigger : ActorFactory
    {
        #region Constructor
        public ActorFactory_Trigger()
        {
            this.nativeActorClassName = "ntrigger_actor";
        }
        #endregion

    }
}

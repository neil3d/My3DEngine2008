using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Nexus.GameFramework;
using NexusEngine;
using NexusEngineExtension;

namespace Nexus.GameFramework
{
    public class GamePlayer : GameAgent
    {
        #region EventHandler
        #endregion

        #region Fields
        /// <summary>
        /// 角色基本动画模型
        /// </summary>
        private NAnimMeshComponent animMesh;
        private NameGenerator<NAnimMeshComponent> animMeshNameGenerator = new NameGenerator<NAnimMeshComponent>();

        #endregion

        #region Properties
        public NAnimMeshComponent AnimMesh
        {
            get { return animMesh; }
        }
        #endregion

        #region Constructor
        public GamePlayer()
        { }
        #endregion

        #region Methods
        virtual public void ResetResource(NResourceLoc loc)
        {
            if (TargetActor != null && animMesh == null)
            {
                NActorComponent comp = TargetActor.CreateComponent(animMeshNameGenerator.GetNextUniqueName(), "nanim_mesh_component");
                animMesh = comp as NAnimMeshComponent;
            }

            animMesh.ResetResource(loc);
        }

        #endregion

    }
}

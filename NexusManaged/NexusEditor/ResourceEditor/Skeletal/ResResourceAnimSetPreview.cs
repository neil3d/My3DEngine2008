using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using NexusEngine;

namespace NexusEditor.ResourceEditor
{
    internal sealed class ResResourceAnimSetPreview : ResourcePreview
    {
        public ResResourceAnimSetPreview(string levelName)
            : base(levelName)
        {
            // 创建可预览对象 
            ReCreateAnimSetPreview();
        }

        public NSkeletalMeshComponent SkeletalMeshComponentPreview
        {
            get { return m_resActorComp as NSkeletalMeshComponent;  }
        }


        internal void ReCreateAnimSetPreview()
        {
            try
            {
                m_level.DestroyAllActors();

                //-- create a actor for mesh
                NActor meshActor = m_level.CreateActor("SkeletalMeshActorPreview", "nactor");
                m_resActorComp = meshActor.CreateComponent("SkeletalMeshComponent", "nskeletal_mesh_component");

                //-- add default light
                AddDefaultLight();
            }
            catch (System.Exception ex)
            {
                NexusEditor.Program.ShowException(ex, "Skeletal Mesh Preview Actor Create FAILED!");
            }
        }

        internal void ResetSkeletalMeshResource(NResourceSkeletalMesh mesh)
        {
            NSkeletalMeshComponent SMC = SkeletalMeshComponentPreview;
            if(SMC == null)
            {
                NLogger.Instance.WriteString(LogType.Debug, "没有创建可预览SkeletalMeshComponent...");
                return;
            }

            try
            {
                if (mesh != null)
                {
                    SMC.ResetResource(mesh, true);
                    m_actorBox = SMC.GetBoundingBox();
                }
            }
            catch (System.Exception e)
            {
                NexusEditor.Program.ShowException(e, "Skeletal Mesh Resource Reset FAILED!");
            }
        }

    }
}

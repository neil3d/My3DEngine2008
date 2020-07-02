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


        internal void ReCreateAnimSetPreview()
        {
            try
            {
                m_level.DestroyAllActors();

                //-- create a actor for mesh
                NActor meshActor = m_level.CreateActor("AnimMeshPreviewActor", "nactor");
                m_resActorComp = meshActor.CreateComponent("AnimMeshComp", "nskeletal_mesh_component");

                //-- add default light
                AddDefaultLight();                
            }
            catch (System.Exception ex)
            {
                NexusEditor.Program.ShowException(ex, "Anim Mesh Preview Actor Create FAILED!");
            }
        }

    }
}

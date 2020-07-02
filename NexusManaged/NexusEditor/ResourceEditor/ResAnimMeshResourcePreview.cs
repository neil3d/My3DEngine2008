using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using NexusEngine;

namespace NexusEditor.ResourceEditor
{
    internal sealed class ResAnimMeshResourcePreview : ResourcePreview
    {
        public ResAnimMeshResourcePreview(string levelName)
            : base(levelName)
        {
        }

        public NAnimMeshComponent AnimMeshPreviewComponent
        {
            get { return m_resActorComp as NAnimMeshComponent; }
        }

        public float CurrentAnimLength
        {
            get
            {
                NAnimMeshComponent previewMC = AnimMeshPreviewComponent;
                if (previewMC != null)
                {
                    return previewMC.CurrentAnimLength;
                }
                return 0;
            }
        }
        public string AnimPlaying
        {
            get
            {
                NAnimMeshComponent previewMC = AnimMeshPreviewComponent;
                if (previewMC != null)
                {
                    return previewMC.AnimPlaying;
                }
                return string.Empty;
            }
        }

        private void CreateAnimMeshPreview()
        {
            try
            {
                m_level.DestroyAllActors();

                //-- create a actor for mesh
                NActor meshActor = m_level.CreateActor("AnimMeshPreviewActor", "nactor");
                m_resActorComp = meshActor.CreateComponent("AnimMeshComp", "nanim_mesh_component");

                //-- add default light
                AddDefaultLight();                
            }
            catch (System.Exception ex)
            {
                NexusEditor.Program.ShowException(ex, "Anim Mesh Preview Actor Create FAILED!");
            }
        }

        internal void ResetAnimMeshResource(NResourceAnimMesh mesh)
        {
            // 重新创建可预览Actor
            CreateAnimMeshPreview();

            NAnimMeshComponent previewMC = AnimMeshPreviewComponent;
            if (previewMC == null)
            {
                NLogger.Instance.WriteString(LogType.Debug, "没有创建可预览对象...");
                return;
            }

            try
            {
                previewMC.ResetResource(mesh);
                m_actorBox = previewMC.GetBoundingBox();                
            }
            catch (System.Exception e)
            {
                NexusEditor.Program.ShowException(e, "Anim Mesh Preview Open FAILED!");
            }
        }

        internal void SetAnim(string sequence)
        {
            NAnimMeshComponent previewMC = AnimMeshPreviewComponent;
            if (previewMC != null)
            {
                previewMC.SetAnim(sequence);
            }
        }

        internal void PlayAnim(bool loop, float rate, float startTime)
        {
            NAnimMeshComponent previewMC = AnimMeshPreviewComponent;
            if (previewMC != null)
            {
                previewMC.PlayAnim(loop, rate, startTime);
            }
        }

        internal void StopAnim()
        {
            NAnimMeshComponent previewMC = AnimMeshPreviewComponent;
            if (previewMC != null)
            {
                previewMC.StopAnim();
            }
        }

        internal void SetPosition(float newPos, bool fireNotifies)
        {
            NAnimMeshComponent previewMC = AnimMeshPreviewComponent;
            if (previewMC != null)
            {
                previewMC.SetPosition(newPos, fireNotifies);
            }
        }
    }
}

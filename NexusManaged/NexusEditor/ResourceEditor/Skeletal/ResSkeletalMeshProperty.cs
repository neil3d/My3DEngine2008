using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Text;
using NexusEngine;

namespace NexusEditor.ResourceEditor
{
    class ResSkeletalMeshProperty : ResProperty
    {
        public ResSkeletalMeshProperty(NResourceSkeletalMesh res)
            : base(res)
        { }

        protected NResourceSkeletalMesh Resource
        {
            get
            {
                return m_resource as NResourceSkeletalMesh;
            }
        }
    }

    class SkeletalMeshMaterialProperty
    {
        private NResourceLoc m_Loc;
        private NResourceSkeletalMesh m_Res;
        private int m_Lod;
        private int m_Section;

        public SkeletalMeshMaterialProperty(NResourceSkeletalMesh res, int lod, int sec)
        {
            m_Res = res; m_Lod = lod; m_Section = sec;
            if (m_Res != null)
            {
                NMtlBase mtl = m_Res.GetMaterial(m_Lod, m_Section);
                if (mtl != null)
                {
                    m_Loc = new NResourceLoc(mtl.Name);
                }
            }
        }

        [Category("Material")]
        public NResourceLoc MaterialName
        {
            get
            {
                return m_Loc;
            }
            set
            {
                m_Loc = value;
                if (m_Res != null)
                {
                    NMtlBase mtl = NMtlBase.FromFile(m_Loc);
                    m_Res.ImportSetMaterial(m_Lod, m_Section, mtl);
                }
            }
        }
    }
}

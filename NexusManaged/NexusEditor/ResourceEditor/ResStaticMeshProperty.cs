using System;
using System.Collections;
using System.ComponentModel;
using System.Drawing.Design;
using System.Windows.Forms.Design;
using NexusEngine;

namespace NexusEditor.ResourceEditor
{
    /// <summary>
    /// Static Mesh编辑器的PropertyGrid显示对象
    /// </summary>
    class ResStaticMeshProperty : ResProperty
    {
        public ResStaticMeshProperty(NResourceStaticMesh res)
            : base(res)
        { }

        protected NResourceStaticMesh Resource
        {
            get
            {
                NResourceStaticMesh ret = m_resource as NResourceStaticMesh;
                return ret;
            }
        }

        [Category("Static Mesh")]
        public MeshLODInfoCollection LODs
        {
            get
            {
                MeshLODInfoCollection lods = new MeshLODInfoCollection();

                NResourceStaticMesh res = this.Resource;
                int numLod = res.GetNumLOD();
                for (int i = 0; i < numLod; i++)
                {
                    MeshLODInfo lod = new MeshLODInfo();

                    int numVert=-1, numSec=-1, numTri=-1;
                    res.GetLODInfo(i, out numVert, out numSec, out numTri);
                    lod.NumVertices = numVert;
                    lod.NumSection = numSec;
                    lod.NumTriangles = numTri;

                    for (int j = 0; j < numSec; j++)
                    {
                        NMtlStatic mtl = res.GetMaterial(i, j) as NMtlStatic;
						if (mtl != null)
						{
							lod.Materials.Add(new NResourceLoc(mtl.Name));
						}
						else
						{
							lod.Materials.Add(new NResourceLoc("",""));
						}
                    }

                    lods.Add(lod);
                }

                return lods;
            }
        }
    }// end of class ResStaticMeshProperty

	class StaticMeshMaterialProperty
	{
		private NResourceLoc m_Loc;
		private NResourceStaticMesh m_Res;
		private int m_Lod;
		private int m_Section;

		public StaticMeshMaterialProperty(NResourceStaticMesh res,int lod,int sec)
		{
			m_Res = res; m_Lod = lod; m_Section = sec;
			if(m_Res!=null)
			{
				NMtlBase mtl = m_Res.GetMaterial(m_Lod, m_Section);
				if(mtl!=null)
				{
					m_Loc = new NResourceLoc(mtl.Name);
				}
			}
		}

		[Category("Material")]
		public NResourceLoc	MaterialName
		{
			get
			{
				return m_Loc;
			}
			set
			{
				m_Loc = value;
				if(m_Res!=null)
				{
					NMtlBase mtl = NMtlBase.FromFile(m_Loc);
					m_Res.ImportSetMaterial(m_Lod, m_Section, mtl);
				}
			}
		}
	}

	class AnimedMeshMaterialProperty
	{
		private NResourceLoc m_Loc;
		private NResourceAnimMesh m_Res;
		private int m_Lod;
		private int m_Section;

		public AnimedMeshMaterialProperty(NResourceAnimMesh res, int lod, int sec)
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

    class ResStaticMeshImportDefault
    {
        private NResourceLoc m_defaultMaterialTemlate;

        [Category("Material Default")]
        public NResourceLoc MaterialTemplateRes
        {
            get { return m_defaultMaterialTemlate; }
            set { m_defaultMaterialTemlate = value; }
        }
    };
}

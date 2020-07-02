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
                        NMaterial mtl = res.GetMaterial(i, j) as NMaterial;
                        MaterialProperty mtlProp = new MaterialProperty(mtl);
                        lod.Materials.Add(mtlProp);
                    }

                    lods.Add(lod);
                }

                return lods;
            }
        }
    }// end of class ResStaticMeshProperty

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

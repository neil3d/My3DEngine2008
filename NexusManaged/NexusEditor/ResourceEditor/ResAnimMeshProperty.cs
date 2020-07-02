using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Text;
using NexusEngine;

namespace NexusEditor.ResourceEditor
{
    /// <summary>
    /// 在PropertyGrid中显示/编辑Anim Mesh资源对象
    /// </summary>
    class ResAnimMeshProperty : ResProperty
    {
        public ResAnimMeshProperty(NResourceAnimMesh res)
            : base(res)
        { }

        protected NResourceAnimMesh Resource
        {
            get
            {
                NResourceAnimMesh ret = m_resource as NResourceAnimMesh;
                return ret;
            }
        }

        [Category("Anim Mesh")]
        public AnimSequenceInfoCollection Sequences
        {
            get
            {
                AnimSequenceInfoCollection seqSet = new AnimSequenceInfoCollection();

                NResourceAnimMesh res = this.Resource;
                if (res.GetNumLOD() > 0)
                {
                    int numSeq = res.GetNumSequence();

                    float seqLen;
                    string seqName;
                    for (int i = 0; i < numSeq; i++)
                    {
                        seqName = res.GetSequenceInfo(i, out seqLen);
                        seqSet.Add(new AnimSequenceInfo(seqName, seqLen));
                    }
                }// end of if

                return seqSet;
            }
        }

        [Category("Anim Mesh")]
        public MeshLODInfoCollection LODs
        {
            get
            {
                MeshLODInfoCollection lods = new MeshLODInfoCollection();

                NResourceAnimMesh res = this.Resource;
                int numLod = res.GetNumLOD();
                for (int i = 0; i < numLod; i++)
                {
                    MeshLODInfo lod = new MeshLODInfo();

                    int numVert = -1, numSec = -1, numTri = -1;
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
    }
}

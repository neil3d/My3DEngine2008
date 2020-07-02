
#include "stdafx.h" 
#include "ExportBaseDef.h"
#include <assert.h>

namespace F3D
{
	bool EdgeEqual(FaceEdge *pEdge, FaceEdge *pEdge2, const Vert_PNT* pVB)
	{
		/*const Vector3& pos0=pVB[pEdge->vert[0]].pos;
		const Vector3& pos1=pVB[pEdge->vert[1]].pos;
		const Vector3& pos_0=pVB[pEdge2->vert[0]].pos;
		const Vector3& pos_1=pVB[pEdge2->vert[1]].pos;
		*/
		const WORD pos0=pEdge->vert[0];
		const WORD pos1=pEdge->vert[1];
		const WORD pos_0=pEdge2->vert[0];
		const WORD pos_1=pEdge2->vert[1];

		/*有这个就ShadowVolume就不正确
		if(pos0 == pos_0 && pos1==pos_1)
		{
			return true;
		}
		*/
		if(pos0 == pos_1 && pos1==pos_0)
			return true;
		return false;
	}

	void MeshTopo::ComputeEdgeConnect(const Vert_PNT *pVB)
	{
		for(WORD i=0;i<faceArray.size();i++)
		{
			FaceTopo& face=faceArray[i];
			for(WORD j=0;j<3;j++)//for each edge
			{
				FaceEdge& edge=face.edges[j];
				int numShare=1;
				for(WORD ii=0;ii<faceArray.size();ii++)
				{
					FaceTopo& face2=faceArray[ii];
					if(i == ii)
						continue;
					for(WORD jj=0;jj<3;jj++)
					{
						if(EdgeEqual(&edge,&face2.edges[jj],pVB))
						{
							assert(numShare < EDGE_MAX_FACE-1);
							edge.face[numShare++]=ii;
						}
					}
				}//endof for
			}
		}
	}
	
}//namespace F3D
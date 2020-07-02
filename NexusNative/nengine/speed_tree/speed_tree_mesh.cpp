#include "StdAfx.h"
#include <sstream>
#include "speed_tree_mesh.h"

namespace nexus
{
	void create_shader_cfg(shader_macro_array& macros)
	{
		ostringstream ostr;
		ostr << c_nNumWindMatricesPerSource;
		macros.push_back(shader_define("SPEEDTREE_NUM_WIND_MATRICES", ostr.str()));
		
		macros.push_back(shader_define("SPEEDTREE_UPVECTOR_Y",""));

		ostr.clear();
		ostr << c_nNumLeafAnglesPerSource;
		macros.push_back(shader_define("SPEEDTREE_MAX_NUM_LEAF_ANGLES", ostr.str()));

#ifdef SPEEDTREE_BRANCH_DETAIL_LAYER
		macros.push_back(shader_define("SPEEDTREE_BRANCH_DETAIL_LAYER",""));
#endif
#ifdef SPEEDTREE_BRANCH_NORMAL_MAPPING
		macros.push_back(shader_define("SPEEDTREE_BRANCH_NORMAL_MAPPING",""));
#endif
#ifdef SPEEDTREE_LEAF_NORMAL_MAPPING
		macros.push_back(shader_define("SPEEDTREE_LEAF_NORMAL_MAPPING",""));
#endif
#ifdef SPEEDTREE_FROND_NORMAL_MAPPING
		macros.push_back(shader_define("SPEEDTREE_FROND_NORMAL_MAPPING",""));
#endif		
#ifdef SPEEDTREE_BRANCH_FADING
		macros.push_back(shader_define("SPEEDTREE_BRANCH_FADING",""));
#endif
	}

	//  Constants
	const float c_fWindWeightCompressionRange = 0.98f;
	const float c_fWeightRange = 0.98f;

	nstring nspt_branch_vertex_data::get_vertex_type_name() const
	{
		return _T("spt_branch_vertex");
	}

	nstring nspt_branch_vertex_data::get_vertex_factory_name() const
	{
		return _T("vfactory_spt_branch");
	}

	int nspt_branch_vertex_data::create(const CSpeedTreeRT::SGeometry& spt)
	{
		const CSpeedTreeRT::SGeometry::SIndexed& sBranches = spt.m_sBranches;

		if( sBranches.m_nNumVertices <= 0)
		{			
			return 0;
		}
		
		//-- alloc vertex buffer
		size_t num_vert = sBranches.m_nNumVertices;
		init_pos_stream(num_vert, true);
		//--
		{
			unsigned short offset = 0;
			element_array packed_array;
			for(int v=0; v<3; v++)
			{
				vertex_element_define vdef = {1, offset, EVET_FLOAT4, EVEU_TexCoord, v};
				offset += sizeof(float)*4;
				packed_array.push_back(vdef);
			}
			
#ifdef SPEEDTREE_BRANCH_FADING
			vertex_element_define vdef = {1, offset, EVET_FLOAT4, EVEU_TexCoord, 3};
#else
			vertex_element_define vdef = {1, offset, EVET_FLOAT3, EVEU_TexCoord, 3};
#endif
			packed_array.push_back(vdef);

			m_packed.init(packed_array, num_vert);
		}

		// are self-shadow coordinates present?
		bool m_bBranchesReceiveShadows = sBranches.m_pTexCoords[CSpeedTreeRT::TL_SHADOW] != NULL;

		// attributes are stored a little strangely in an attempt to pack as many in as possible
		int i = 0;

		// .xyz = coords
		// .w = self-shadow s-texcoord
		nASSERT(sBranches.m_pCoords);
		for (i = 0; i < sBranches.m_nNumVertices; ++i)
		{
			float fSelfShadowCoord_S = m_bBranchesReceiveShadows ? sBranches.m_pTexCoords[CSpeedTreeRT::TL_SHADOW][i * 2 + 0] : 0.0f;
			m_pos_stream.set_element(i, 0,
				vector4(sBranches.m_pCoords[i * 3 + 0], 
				sBranches.m_pCoords[i * 3 + 1], 
				sBranches.m_pCoords[i * 3 + 2], 
				fSelfShadowCoord_S)
				);
		}

		// .xyz = normal
		// .w = self-shadow t-texcoord
		nASSERT(sBranches.m_pNormals);
		for (i = 0; i < sBranches.m_nNumVertices; ++i)
		{
			float fSelfShadowCoord_T = m_bBranchesReceiveShadows ? sBranches.m_pTexCoords[CSpeedTreeRT::TL_SHADOW][i * 2 + 1] : 0.0f;
			m_packed.set_element(i, 0,
				vector4(sBranches.m_pNormals[i * 3 + 0], 
				sBranches.m_pNormals[i * 3 + 1], 
				sBranches.m_pNormals[i * 3 + 2], 
				fSelfShadowCoord_T)
				);
		}

		// .xy = diffuse texcoords (every tree must have a diffuse layer - fatal error if not)
		// .zw = wind parameters
		nASSERT(sBranches.m_pTexCoords[CSpeedTreeRT::TL_DIFFUSE]);
		nASSERT(sBranches.m_pWindMatrixIndices[0] && sBranches.m_pWindWeights[0] && sBranches.m_pWindMatrixIndices[1] && sBranches.m_pWindWeights[1]);
		for (i = 0; i < sBranches.m_nNumVertices; ++i)
		{
			const float* pDiffuseCoords = sBranches.m_pTexCoords[CSpeedTreeRT::TL_DIFFUSE] + i * 2;

			// each vertex has four wind parameters:
			//   - wind matrix indices #1 and #2
			//   - wind weight #1 and #2 (range 0.0 to 1.0)
			//
			// they are uploaded as two parameters: (index1 + weight1) and (index2 + weight2).
			// the weights are scaled back from the [0.0, 1.0] range to something like [0.0, 0.98] so that
			// they will always be the fractional component of the sum.  The scale value is c_fWeightRange.
			//
			// these values are used in the WindEffect*() shader functions in Utility.fx

			// the SpeedTreeRT library should have already clamped the weights to [0.0,1.0]
			nASSERT(sBranches.m_pWindWeights[0][i] >= 0.0f && sBranches.m_pWindWeights[0][i] <= 1.0f);
			nASSERT(sBranches.m_pWindWeights[1][i] >= 0.0f && sBranches.m_pWindWeights[0][i] <= 1.0f);
			float fWindParam1 = sBranches.m_pWindMatrixIndices[0][i] + sBranches.m_pWindWeights[0][i] * c_fWeightRange;
			float fWindParam2 = sBranches.m_pWindMatrixIndices[1][i] + sBranches.m_pWindWeights[1][i] * c_fWeightRange;

			m_packed.set_element(i, 1,
				vector4(pDiffuseCoords[0], pDiffuseCoords[1], fWindParam1, fWindParam2)
				);
		}

		// normal-map texcoords
		// .xy = detail texcoords
		// .zw = normal-map texcoords
		for (i = 0; i < sBranches.m_nNumVertices; ++i)
		{
#ifdef SPEEDTREE_BRANCH_DETAIL_LAYER
			const float* pDetailCoords = sBranches.m_pTexCoords[CSpeedTreeRT::TL_DETAIL] + i * 2;
#else
			float pDetailCoords[2] = { 0.0f, 0.0f };
#endif
#ifdef SPEEDTREE_BRANCH_NORMAL_MAPPING
			const float* pNormalMapCoords = sBranches.m_pTexCoords[CSpeedTreeRT::TL_NORMAL] + i * 2;
#else
			float pNormalMapCoords[2] = { 0.0f, 0.0f };
#endif
			m_packed.set_element(i, 2, 
				vector4(pDetailCoords[0], pDetailCoords[1], pNormalMapCoords[0], pNormalMapCoords[1])
				);
		}

		// tangents (binormals are derived in the vertex shader)
		// .xyz = tangent
		// .w = lod fade hint
		for (i = 0; i < sBranches.m_nNumVertices; ++i)
		{
#ifdef SPEEDTREE_BRANCH_NORMAL_MAPPING
			nASSERT(sBranches.m_pTangents);
			float afTexCoords[ ] = 
			{ 
				sBranches.m_pTangents[i * 3 + 0],
				sBranches.m_pTangents[i * 3 + 1],
				sBranches.m_pTangents[i * 3 + 2]
#ifdef SPEEDTREE_BRANCH_FADING
				,sBranches.m_pLodFadeHints[i]
#endif
			};
#else // SPEEDTREE_BRANCH_NORMAL_MAPPING
			float afTexCoords[] = 
			{ 
				0.0f, 0.0f, 0.0f
#ifdef SPEEDTREE_BRANCH_FADING
				,sBranches.m_pLodFadeHints[i]
#endif
			};
#endif // SPEEDTREE_BRANCH_NORMAL_MAPPING

#ifdef SPEEDTREE_BRANCH_FADING
			m_packed.set_element(i, 3, 
				vector4(afTexCoords[0],afTexCoords[1],afTexCoords[2],afTexCoords[3])
			);
#else
			m_packed.set_element(i, 3, 
				vector3(afTexCoords[0],afTexCoords[1],afTexCoords[2])
				);
#endif
		}

		//-- shader config
		create_shader_cfg(m_macro_array);

		return num_vert;
	}

	int nspt_branch_vertex_data::get_triangle_strip(const CSpeedTreeRT::SGeometry& spt, int nLod, index_buffer16& indices)
	{
		const CSpeedTreeRT::SGeometry::SIndexed& sBranches = spt.m_sBranches;
		nASSERT(sBranches.m_pNumStrips);

		int nNumVertsInStrip = 0;
		if (sBranches.m_pNumStrips[nLod] == 1)
		{
			nASSERT(sBranches.m_pStripLengths[nLod]);

			nNumVertsInStrip = sBranches.m_pStripLengths[nLod][0];
			if (nNumVertsInStrip > 2)
			{
				for (int i = 0; i < nNumVertsInStrip; ++i)
				{
					nASSERT(sBranches.m_pStrips[nLod][0][i] > -1 && sBranches.m_pStrips[nLod][0][i] < sBranches.m_nNumVertices);
					int index = sBranches.m_pStrips[nLod][0][i];
					nASSERT(index>=0 && index<std::numeric_limits<unsigned short>::max());
					indices.append_index(index);
				}
			}
			return sBranches.m_pStripLengths[nLod][0];
		}

		return nNumVertsInStrip;
	}

	nstring nspt_frond_vertex_data::get_vertex_type_name() const
	{
		return _T("spt_frond_vertex");
	}

	nstring nspt_frond_vertex_data::get_vertex_factory_name() const
	{
		return _T("vfactory_spt_frond");
	}

	int nspt_frond_vertex_data::create(const CSpeedTreeRT::SGeometry& spt)
	{
		// query vertex attribute data		
		const CSpeedTreeRT::SGeometry::SIndexed& sFronds = spt.m_sFronds;

#ifdef SPEEDTREE_BRANCH_FADING
		// get transition radius
		float m_fFrondFadeDistance = sFronds.m_fLodFadeDistance;
#endif
		// are self-shadow coordinates present?
		bool m_bFrondsReceiveShadows = sFronds.m_pTexCoords[CSpeedTreeRT::TL_SHADOW] != NULL;

		if( sFronds.m_nNumVertices <= 0 )
			return 0;

		//-- alloc vertex buffer
		size_t num_vert = sFronds.m_nNumVertices;
		init_pos_stream(num_vert, true);
		//--
		{
			unsigned short offset = 0;
			element_array packed_array;
			for(int v=0; v<3; v++)
			{
				vertex_element_define vdef = {1, offset, EVET_FLOAT4, EVEU_TexCoord, v};
				offset += sizeof(float)*4;
				packed_array.push_back(vdef);
			}
			m_packed.init(packed_array, num_vert);
		}

		// .xyz = coords
		// .w = self-shadow s-texcoord
		nASSERT(sFronds.m_pCoords);
		int i = 0;
		for (i = 0; i < sFronds.m_nNumVertices; ++i)
		{
			float fSelfShadowCoord_S = m_bFrondsReceiveShadows ? sFronds.m_pTexCoords[CSpeedTreeRT::TL_SHADOW][i * 2 + 0] : 0.0f;
			m_pos_stream.set_element(i, 0,
				vector4(sFronds.m_pCoords[i * 3 + 0], sFronds.m_pCoords[i * 3 + 1], sFronds.m_pCoords[i * 3 + 2], fSelfShadowCoord_S)
				);
		}

		// .xyz = normal
		// .w = self-shadow t-texcoord
		nASSERT(sFronds.m_pNormals);
		for (i = 0; i < sFronds.m_nNumVertices; ++i)
		{
			float fSelfShadowCoord_T = m_bFrondsReceiveShadows ? sFronds.m_pTexCoords[CSpeedTreeRT::TL_SHADOW][i * 2 + 1] : 0.0f;
			m_packed.set_element(i, 0,
				vector4(sFronds.m_pNormals[i * 3 + 0], sFronds.m_pNormals[i * 3 + 1], sFronds.m_pNormals[i * 3 + 2], fSelfShadowCoord_T)
				);
		}

		// .xy = diffuse texcoords (every tree must have a diffuse layer - fatal error if not)
		// .zw = wind parameters
		nASSERT(sFronds.m_pTexCoords[CSpeedTreeRT::TL_DIFFUSE]);
		nASSERT(sFronds.m_pWindMatrixIndices[0] && sFronds.m_pWindWeights[0] && sFronds.m_pWindMatrixIndices[1] && sFronds.m_pWindWeights[1]);
		for (i = 0; i < sFronds.m_nNumVertices; ++i)
		{
			const float* pDiffuseCoords = sFronds.m_pTexCoords[CSpeedTreeRT::TL_DIFFUSE] + i * 2;

			// each vertex has four wind parameters:
			//   - wind matrix indices #1 and #2
			//   - wind weight #1 and #2 (range 0.0 to 1.0)
			//
			// they are uploaded as two parameters: (index1 + weight1) and (index2 + weight2).
			// the weights are scaled back from the [0.0, 1.0] range to something like [0.0, 0.98] so that
			// they will always be the fractional component of the sum.  The scale value is c_fWeightRange.
			//
			// these values are used in the WindEffect*() shader functions in Utility.fx

			// the SpeedTreeRT library should have already clamped the weights to [0.0,1.0]
			nASSERT(sFronds.m_pWindWeights[0][i] >= 0.0f && sFronds.m_pWindWeights[0][i] <= 1.0f);
			nASSERT(sFronds.m_pWindWeights[1][i] >= 0.0f && sFronds.m_pWindWeights[0][i] <= 1.0f);
			float fWindParam1 = sFronds.m_pWindMatrixIndices[0][i] + sFronds.m_pWindWeights[0][i] * c_fWeightRange;
			float fWindParam2 = sFronds.m_pWindMatrixIndices[1][i] + sFronds.m_pWindWeights[1][i] * c_fWeightRange;
			
			m_packed.set_element(i, 1, 
				vector4(pDiffuseCoords[0], pDiffuseCoords[1], fWindParam1, fWindParam2)
				);
		}

#if defined(SPEEDTREE_BRANCH_FADING) || defined(SPEEDTREE_FROND_NORMAL_MAPPING)
		for (i = 0; i < sFronds.m_nNumVertices; ++i)
		{
#ifdef SPEEDTREE_BRANCH_FADING
			nASSERT(sFronds.m_pLodFadeHints);
			float fFadeHint = sFronds.m_pLodFadeHints[i];
#else
			float fFadeHint = -1.0f;
#endif
#ifdef SPEEDTREE_FROND_NORMAL_MAPPING
			const float* pTangent = sFronds.m_pTangents + i * 3;
#else
			const float pTangent[3] = { 0.0f, 0.0f, 0.0f };
#endif
			const float afLayerData[4] = { pTangent[0], pTangent[1], pTangent[2], fFadeHint };
			m_packed.set_element(i, 2, 
				vector4(afLayerData[0],afLayerData[1],afLayerData[2],afLayerData[3])
				);
		}
#endif

		//-- shader config
		create_shader_cfg(m_macro_array);

		return num_vert;
	}

	int nspt_frond_vertex_data::get_triangle_strip(const CSpeedTreeRT::SGeometry& spt, int nLod, index_buffer16& indices)
	{
		const CSpeedTreeRT::SGeometry::SIndexed& sFronds = spt.m_sFronds;

		nASSERT(sFronds.m_pNumStrips);
		int nNumVertsInStrip = 0;
		if (sFronds.m_pNumStrips[nLod] == 1)
		{
			nASSERT(sFronds.m_pStripLengths[nLod]);
			
			nNumVertsInStrip = sFronds.m_pStripLengths[nLod][0];
			if (nNumVertsInStrip <= 2)
				return 0;

			for (int i = 0; i < nNumVertsInStrip; ++i)
			{
				nASSERT(sFronds.m_pStrips[nLod][0][i] > -1 && sFronds.m_pStrips[nLod][0][i] < sFronds.m_nNumVertices);

				int index = sFronds.m_pStrips[nLod][0][i];
				nASSERT(index>=0 && index<std::numeric_limits<unsigned short>::max());
				indices.append_index(index);
			}

			return nNumVertsInStrip;
		}

		return nNumVertsInStrip;
	}

	nstring nspt_leafcard_vertex_data::get_vertex_type_name() const
	{
		return _T("spt_leafcard_vertex");
	}

	nstring nspt_leafcard_vertex_data::get_vertex_factory_name() const
	{
		return _T("vfactory_spt_leafcard");
	}

	int nspt_leafcard_vertex_data::create(const CSpeedTreeRT::SGeometry::SLeaf& sLeaves)
	{
		const int nNumLeaves = sLeaves.m_nNumLeaves;

		//--
		int num_vert = 0;
		for (int nLeaf = 0; nLeaf < nNumLeaves; ++nLeaf)
		{
			const CSpeedTreeRT::SGeometry::SLeaf::SCard* pCard = sLeaves.m_pCards + sLeaves.m_pLeafCardIndices[nLeaf];
			if (!pCard->m_pMesh)
				num_vert += 6;
		}
		if( num_vert <= 0)
			return 0;

		init_pos_stream(num_vert, true);
		
		{
			unsigned short offset = 0;
			element_array packed_array;
			vertex_element_define ndef = {1, offset, EVET_FLOAT3, EVEU_Normal, 0};
			offset += sizeof(float)*3;
			packed_array.push_back(ndef);

			int v;
			for(v=0; v<3; v++)
			{
				vertex_element_define vdef = {1, offset, EVET_FLOAT4, EVEU_TexCoord, v};
				offset += sizeof(float)*4;
				packed_array.push_back(vdef);
			}
#ifdef SPEEDTREE_LEAF_NORMAL_MAPPING
			vertex_element_define vdef = {1, offset, EVET_FLOAT3, EVEU_TexCoord, v};			
			packed_array.push_back(vdef);
#endif
			m_packed.init(packed_array, num_vert);
		}

		//--
		// make sure the required components are intact
		nASSERT(sLeaves.m_pCards);
		nASSERT(sLeaves.m_pWindMatrixIndices[0] && sLeaves.m_pWindMatrixIndices[1] && sLeaves.m_pWindWeights[0] && sLeaves.m_pWindWeights[1]);

		// Standard DirectX 9.0c does not support quad primitive rendering so we'll need to setup a triangle list
		const int anVertIndices[6] = { 0, 1, 2, 0, 2, 3 };

		int nNumLeafCards = 0;
		int v = 0;
		for (int nLeaf = 0; nLeaf < nNumLeaves; ++nLeaf)
		{
			const CSpeedTreeRT::SGeometry::SLeaf::SCard* pCard = sLeaves.m_pCards + sLeaves.m_pLeafCardIndices[nLeaf];
			if (!pCard->m_pMesh)
			{
				++nNumLeafCards;				

				for (int nVert = 0; nVert < 6; ++nVert)
				{
					int nCorner = anVertIndices[nVert];

					// each vertex has four wind parameters:
					//   - wind matrix indices #1 and #2
					//   - wind weight #1 and #2 (range 0.0 to 1.0)
					//
					// they are uploaded as two parameters: (index1 + weight1) and (index2 + weight2).
					// the weights are scaled back from the [0.0, 1.0] range to something like [0.0, 0.98] so that
					// they will always be the fractional component of the sum.  The scale value is c_fWeightRange.
					//
					// these values are used in the WindEffect*() shader functions in Utility.fx

					float fWindParam1 = sLeaves.m_pWindMatrixIndices[0][nLeaf] + sLeaves.m_pWindWeights[0][nLeaf] * c_fWeightRange;
					float fWindParam2 = sLeaves.m_pWindMatrixIndices[1][nLeaf] + sLeaves.m_pWindWeights[1][nLeaf] * c_fWeightRange;
					m_packed.set_element(v, 1,
						vector4(pCard->m_pTexCoords[nCorner * 2], pCard->m_pTexCoords[nCorner * 2 + 1], fWindParam1, fWindParam2)
						);					

					// tex layer 1: .x = width, .y = height, .z = pivot x, .w = pivot.y
					m_packed.set_element(v, 2,
						vector4(pCard->m_fWidth, pCard->m_fHeight, pCard->m_afPivotPoint[0] - 0.5f, pCard->m_afPivotPoint[1] - 0.5f)
						);					

					// tex layer 2: .x = angle.x, .y = angle.y, .z = leaf angle index [0,c_nNumLeafAnglesPerSource-1], .w = dimming
					m_packed.set_element(v, 3, 
						vector4(nDegToRad(pCard->m_afAngleOffsets[0]), nDegToRad(pCard->m_afAngleOffsets[1]),
						float(nLeaf % c_nNumLeafAnglesPerSource), sLeaves.m_pDimming[nLeaf])
						);					

#ifdef SPEEDTREE_LEAF_NORMAL_MAPPING
					// tangent for normal mapping
					const float* afTexCoords = sLeaves.m_pTangents + nLeaf * 12 + nCorner * 3;
					m_packed.set_element(v, 4,
						vector3(afTexCoords[0], afTexCoords[1], afTexCoords[2])
						);
#endif

					// normal
					const float* afNormal = sLeaves.m_pNormals + nLeaf * 12 + nCorner * 3;
					m_packed.set_element(v, 0,
						vector3(afNormal[0], afNormal[1], afNormal[2])
						);

					// coordinate
					m_pos_stream.set_element(v, 0,
						vector4(sLeaves.m_pCenterCoords[nLeaf * 3 + 0], sLeaves.m_pCenterCoords[nLeaf * 3 + 1], sLeaves.m_pCenterCoords[nLeaf * 3 + 2], float(nCorner))
						);

					v++;
				}// end of for(vert)
			}// end of if
		}//end of for(leaf)

		//-- shader config
		create_shader_cfg(m_macro_array);

		return num_vert;
	}

	nstring nspt_leafmesh_vertex_data::get_vertex_type_name() const
	{
		return _T("spt_leafmesh_vertex");
	}

	nstring nspt_leafmesh_vertex_data::get_vertex_factory_name() const
	{
		return _T("vfactory_spt_leafmesh");
	}

	int nspt_leafmesh_vertex_data::create(const CSpeedTreeRT::SGeometry::SLeaf& sLeaves, index_buffer16& indices)
	{
		const int nNumLeaves = sLeaves.m_nNumLeaves;

		//--
		int num_vert = 0;
		for (int nLeaf = 0; nLeaf < nNumLeaves; ++nLeaf)
		{
			const CSpeedTreeRT::SGeometry::SLeaf::SCard* pCard = sLeaves.m_pCards + sLeaves.m_pLeafCardIndices[nLeaf];
			if (pCard->m_pMesh)
				num_vert += pCard->m_pMesh->m_nNumVertices;
		}
		if( num_vert <= 0)
			return 0;

		init_pos_stream(num_vert, true);

		{
			element_array packed_array;
			vertex_element_define ndef = {1, 0, EVET_FLOAT3, EVEU_Normal, 0};
			packed_array.push_back(ndef);

			vertex_element_define t0def = {1, 0, EVET_FLOAT4, EVEU_TexCoord, 0};
			packed_array.push_back(t0def);

			vertex_element_define t1def = {1, 0, EVET_FLOAT3, EVEU_TexCoord, 1};
			packed_array.push_back(t1def);

			vertex_element_define t2def = {1, 0, EVET_FLOAT3, EVEU_TexCoord, 2};
			packed_array.push_back(t2def);

			vertex_element_define t3def = {1, 0, EVET_FLOAT4, EVEU_TexCoord, 3};
			packed_array.push_back(t3def);

#ifdef SPEEDTREE_LEAF_NORMAL_MAPPING
			vertex_element_define t4def = {1, 0, EVET_FLOAT3, EVEU_TexCoord, 4};
			packed_array.push_back(t4def);
#endif	
			m_packed.init(packed_array, num_vert);
		}

		//--
		const CSpeedTreeRT::SGeometry::SLeaf* pLod = &sLeaves;

		int v = 0;

		for (int nLeaf = 0; nLeaf < pLod->m_nNumLeaves; ++nLeaf)
		{
			const CSpeedTreeRT::SGeometry::SLeaf::SCard* pCard = pLod->m_pCards + pLod->m_pLeafCardIndices[nLeaf];
			const CSpeedTreeRT::SGeometry::SLeaf::SMesh* pMesh = pCard->m_pMesh;

			// does this cluster have a mesh associated with it?
			if (pMesh)
			{
				const int c_nVertexTableStart = v;

				// setup the vertex attributes
				for (int nVertex = 0; nVertex < pMesh->m_nNumVertices; ++nVertex)
				{
					// each vertex has four wind parameters:
					//   - wind matrix indices #1 and #2
					//   - wind weight #1 and #2 (range 0.0 to 1.0)
					//
					// they are uploaded as two parameters: (index1 + weight1) and (index2 + weight2).
					// the weights are scaled back from the [0.0, 1.0] range to something like [0.0, 0.98] so that
					// they will always be the fractional component of the sum.  The scale value is c_fWeightRange.
					//
					// these values are used in the WindEffect*() shader functions in Utility.fx

					float fWindParam1 = pLod->m_pWindMatrixIndices[0][nLeaf] + pLod->m_pWindWeights[0][nLeaf] * c_fWeightRange;
					float fWindParam2 = pLod->m_pWindMatrixIndices[1][nLeaf] + pLod->m_pWindWeights[1][nLeaf] * c_fWeightRange;

					// tex layer 0: .xy = diffuse texcoords, .z = leaf angle index [0,c_nNumLeafAnglesPerSource-1], .w = dimming
					m_packed.set_element(v, 1, 
						vector4(pMesh->m_pTexCoords[nVertex * 2], pMesh->m_pTexCoords[nVertex * 2 + 1],
						float(nLeaf % c_nNumLeafAnglesPerSource), pLod->m_pDimming[nLeaf])
						);

					// orientation vectors
					m_packed.set_element(v, 2,
						vector3(-pLod->m_pTangents[nLeaf * 12 + 0], pLod->m_pNormals[nLeaf * 12 + 0], pLod->m_pBinormals[nLeaf * 12 + 0])
						);
					m_packed.set_element(v, 3,
						vector3(-pLod->m_pTangents[nLeaf * 12 + 2], pLod->m_pNormals[nLeaf * 12 + 2], pLod->m_pBinormals[nLeaf * 12 + 2])
						);

					// offset (used to position the mesh on the tree)
					m_packed.set_element(v, 4,
						vector4(pLod->m_pCenterCoords[nLeaf * 3 + 0], pLod->m_pCenterCoords[nLeaf * 3 + 1], pLod->m_pCenterCoords[nLeaf * 3 + 2], fWindParam2)
						);

#ifdef SPEEDTREE_LEAF_NORMAL_MAPPING
					// tangent for normal mapping
					const float* pt = pMesh->m_pTangents + nVertex * 3;
					m_packed.set_element(v, 5,
						vector3(pt[0], pt[1], pt[2])
						);
#endif

					// normal
					const float* pn = pMesh->m_pNormals + nVertex * 3;
					m_packed.set_element(v, 0,
						vector3(pn[0],pn[1],pn[2]));					

					// coordinate (3d coord of leaf mesh)
					m_pos_stream.set_element(v, 0,
						vector4(pMesh->m_pCoords[nVertex * 3 + 0] * pCard->m_fMeshScale, 
						pMesh->m_pCoords[nVertex * 3 + 1] * pCard->m_fMeshScale, 
						pMesh->m_pCoords[nVertex * 3 + 2] * pCard->m_fMeshScale, fWindParam1)
						);
					v++;
				}

				// setup the triangle indices
				for (int nIndex = 0; nIndex < pMesh->m_nNumIndices; ++nIndex)
				{
					int index = c_nVertexTableStart + pMesh->m_pIndices[nIndex];
					nASSERT(index>=0 && index<std::numeric_limits<unsigned short>::max());
					indices.append_index(index);
				}					
			}// end of if
		}// end of for(leaf)

		//-- shader config
		create_shader_cfg(m_macro_array);

		return v;
	}
}//namespace nexus
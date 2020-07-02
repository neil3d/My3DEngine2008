#include "StdAfx.h"
#include "ActorXFile.h"

bool PSKFile::load_from_file(const nstring& file_name)
{
	bool ret = true;
	FILE* fp = _tfopen(file_name.c_str(), _T("rb"));

	VChunkHdr header;
	
	//-- file header
	fread(&header, sizeof(VChunkHdr), 1, fp);
	if(strcmp(header.ChunkID, "ACTRHEAD") != 0)
	{
		ret = false;
		goto EXIT_LOAD;
	}

	//-- Skin: 3D Points
	fread(&header, sizeof(VChunkHdr), 1, fp);
	if(strcmp(header.ChunkID, "PNTS0000") != 0)
	{
		ret = false;
		goto EXIT_LOAD;
	}

	nASSERT(header.DataSize == sizeof(VPoint));
	for(int i=0; i<header.DataCount; i++)
	{
		VPoint pt;
		fread(&pt, sizeof(VPoint), 1, fp);
		m_points.push_back(pt);
	}
	
	//-- Skin: VERTICES (wedges)
	fread(&header, sizeof(VChunkHdr), 1, fp);
	if(strcmp(header.ChunkID, "VTXW0000") != 0)
	{
		ret = false;
		goto EXIT_LOAD;
	}

	nASSERT(header.DataSize == sizeof(VVertex));
	for(int i=0; i<header.DataCount; i++)
	{
		VVertex vt;
		fread(&vt, sizeof(VVertex), 1, fp);
		m_vertices.push_back(vt);
	}

	//-- Skin: TRIANGLES (faces)
	fread(&header, sizeof(VChunkHdr), 1, fp);
	if(strcmp(header.ChunkID, "FACE0000") != 0)
	{
		ret = false;
		goto EXIT_LOAD;
	}

	nASSERT(header.DataSize == sizeof(VTriangle));
	for(int i=0; i<header.DataCount; i++)
	{
		VTriangle face;
		fread(&face, sizeof(VTriangle), 1, fp);
		m_faces.push_back(face);
	}

	//-- Skin: Materials
	fread(&header, sizeof(VChunkHdr), 1, fp);
	if(strcmp(header.ChunkID, "MATT0000") != 0)
	{
		ret = false;
		goto EXIT_LOAD;
	}

	nASSERT(header.DataSize == sizeof(VMaterial));
	for(int i=0; i<header.DataCount; i++)
	{
		VMaterial mtl;
		fread(&mtl, sizeof(VMaterial), 1, fp);
		m_mtl_array.push_back(mtl);
	}

	//-- Reference Skeleton: Refskeleton.TotalBones times a VBone.
	fread(&header, sizeof(VChunkHdr), 1, fp);
	if(strcmp(header.ChunkID, "REFSKELT") != 0)
	{
		ret = false;
		goto EXIT_LOAD;
	}

	nASSERT(header.DataSize == sizeof(FNamedBoneBinary));
	for(int i=0; i<header.DataCount; i++)
	{
		FNamedBoneBinary bone;
		fread(&bone, sizeof(FNamedBoneBinary), 1, fp);
		m_skeleton_bones.push_back(bone);
	}

	//-- Reference Skeleton: Refskeleton.TotalBones times a VBone.
	fread(&header, sizeof(VChunkHdr), 1, fp);
	if(strcmp(header.ChunkID, "RAWWEIGHTS") != 0)
	{
		ret = false;
		goto EXIT_LOAD;
	}

	nASSERT(header.DataSize == sizeof(VRawBoneInfluence));
	for(int i=0; i<header.DataCount; i++)
	{
		VRawBoneInfluence weight;
		fread(&weight, sizeof(VRawBoneInfluence), 1, fp);
		m_raw_weights.push_back(weight);
	}

	

EXIT_LOAD:
	fclose(fp);
	return ret;
}

bool PSAFile::load_from_file(const nstring& file_name)
{
	//
	// Animation layout:  
	//
	// name        variable										type
	//
	// ANIMHEAD
	// BONENAMES   RefSkeletonBones								FNamedBoneBinary
	// ANIMINFO    OutAnims										AnimInfoBinary
	// ANIMKEYS    OutAnims[0-.KeyTrack.Num()].KeyTrack[i]....  VQuatAnimKey

	//--
	bool ret = true;

	FILE* fp = _tfopen(file_name.c_str(), _T("rb"));

	VChunkHdr header;

	//-- file header
	fread(&header, sizeof(VChunkHdr), 1, fp);
	if(strcmp(header.ChunkID, "ANIMHEAD") != 0)
	{
		ret = false;
		goto EXIT_LOAD;
	}

	// Bone names (+flags) list:
	fread(&header, sizeof(VChunkHdr), 1, fp);
	if(strcmp(header.ChunkID, "BONENAMES") != 0)
	{
		ret = false;
		goto EXIT_LOAD;
	}

	nASSERT(header.DataSize == sizeof(FNamedBoneBinary));
	for(int i=0; i<header.DataCount; i++)
	{
		FNamedBoneBinary bone;
		fread(&bone, sizeof(FNamedBoneBinary), 1, fp);
		m_skeleton_bones.push_back(bone);
	}

	// animations
	fread(&header, sizeof(VChunkHdr), 1, fp);
	if(strcmp(header.ChunkID, "ANIMINFO") != 0)
	{
		ret = false;
		goto EXIT_LOAD;
	}

	nASSERT(header.DataSize == sizeof(VAnimInfoBinary));
	m_anims.resize(header.DataCount);

	for(int i=0; i<header.DataCount; i++)
	{
		VAnimInfoBinary& anim_info = m_anims[i].AnimInfo;
		fread(&anim_info, sizeof(VAnimInfoBinary), 1, fp);
	}

	fread(&header, sizeof(VChunkHdr), 1, fp);
	if(strcmp(header.ChunkID, "ANIMKEYS") != 0)
	{
		ret = false;
		goto EXIT_LOAD;
	}

	nASSERT(header.DataSize == sizeof(VQuatAnimKey));
	// verify if total matches read keys...
	int total_keys = header.DataCount;
	int read_keys = 0;
	
	for(size_t i=0; i<m_anims.size(); i++)
	{
		int track_keys = m_anims[i].AnimInfo.NumRawFrames * m_anims[i].AnimInfo.TotalBones;
		m_anims[i].KeyTrack.resize(track_keys);
		for(int j=0; j<track_keys; j++)
		{
			VQuatAnimKey& key = m_anims[i].KeyTrack[j];
			fread(&key, sizeof(VQuatAnimKey), 1, fp);
		}
		read_keys += track_keys;
	}

	nASSERT( read_keys == total_keys );

	fread(&header, sizeof(VChunkHdr), 1, fp);
	if(strcmp(header.ChunkID, "SCALEKEYS") != 0)
	{
		ret = false;
		goto EXIT_LOAD;
	}

	nASSERT(header.DataSize == sizeof(VScaleAnimKey));
	total_keys = header.DataCount;
	read_keys = 0;

	for(size_t i=0; i<m_anims.size(); i++)
	{
		if( m_anims[i].AnimInfo.ScaleInclude ) // ONLY for those sequences with scaling.
		{
			int track_keys = m_anims[i].AnimInfo.NumRawFrames * m_anims[i].AnimInfo.TotalBones;
			m_anims[i].ScaleTrack.resize(track_keys);
			for(int j=0; j<track_keys; j++)
			{
				VScaleAnimKey& key = m_anims[i].ScaleTrack[j];
				fread(&key, sizeof(VScaleAnimKey), 1, fp);
			}
			read_keys += track_keys;
		}
	}//end of for

	nASSERT( read_keys == total_keys );

EXIT_LOAD:
	fclose(fp);
	return ret;
}
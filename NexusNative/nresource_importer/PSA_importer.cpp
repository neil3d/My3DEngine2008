#include "StdAfx.h"
#include "PSA_importer.h"
#include <boost/algorithm/string.hpp>
#include "F3D_util.h"

namespace nexus
{
	nDEFINE_CLASS(PSA_importer, nskeletal_anim_importer)

	PSA_importer::PSA_importer(void)
	{
	}

	PSA_importer::~PSA_importer(void)
	{
	}

	void PSA_importer::import_from_file(const nstring& full_path, data& out_data)
	{
		m_file_data.clear();
		if( !m_file_data.load_from_file(full_path) )
			nthrow2(_T("PSA file load failed"), full_path.c_str());

		process_skeleton(out_data.skeleton);

		for(size_t i=0; i<m_file_data.m_anims.size(); i++)
		{
			nskeletal_anim_sequence::ptr new_seq = process_sequence(m_file_data.m_anims[i]);
			if(new_seq
				&& new_seq->m_tracks.size() > 0)
				out_data.anim_array.push_back( new_seq );
		}

		nLog_Info(_T("%s imported, skeleton bone num =%d, num anim sequneces = %d.\r\n"), 
			full_path.c_str(), out_data.skeleton.m_bones.size(), out_data.anim_array.size());
	}

	void PSA_importer::process_skeleton(nskeleton_define& out_skel)
	{
		out_skel.m_bones.clear();

		out_skel.m_bones.resize(m_file_data.m_skeleton_bones.size());
		for(size_t i=0; i<m_file_data.m_skeleton_bones.size(); i++)
		{
			const FNamedBoneBinary& vbone = m_file_data.m_skeleton_bones[i];
			bone_info& nbone = out_skel.m_bones[i];

			std::string bone_name = vbone.Name;
			boost::algorithm::trim(bone_name);

			nbone.name			= conv_string(bone_name);
			nbone.num_children	= vbone.NumChildren;
			nbone.parent_index	= vbone.ParentIndex;
			nbone.pos.position	= _MAX_TO_D3D(vbone.BonePos.Position);
			nbone.pos.orientation = quat_normalize( _MAX_TO_D3D(vbone.BonePos.Orientation) );						
		}

		if( out_skel.m_bones.size() > 0)
		{
			// root bone
			bone_info& root_bone = out_skel.m_bones[0];
			root_bone.parent_index = -1;

			/**
			 *	在ActorX插件中的“int SceneIFC::DigestSkeleton(VActor *Thing)”
				 if ( SerialTree[i].ParentIndex == 0 ) //#check whether root
				 {
				 // #debug #inversion
				 // Root XYZ needs to be negated to match the handedness of the reference skin, and space in Unreal in general.
				 ThisBone.BonePos.Orientation = FQuat(-localAff.q.x,-localAff.q.y,-localAff.q.z,localAff.q.w);
				 }
			*/
			root_bone.pos.orientation.x *= -1;
			root_bone.pos.orientation.y *= -1;
			root_bone.pos.orientation.z *= -1;
		}

		out_skel.init_ref_base();
	}



	nskeletal_anim_sequence::ptr PSA_importer::process_sequence(const VAnimation& vanim)
	{
		nskeletal_anim_sequence::ptr new_seq( new nskeletal_anim_sequence );

		std::string seq_name = vanim.AnimInfo.Name;
		boost::algorithm::trim(seq_name);

		new_seq->m_name			=	conv_string(seq_name);
		new_seq->m_num_frame	=	vanim.AnimInfo.NumRawFrames;
		new_seq->m_length		=	vanim.AnimInfo.TrackTime/vanim.AnimInfo.AnimRate;

		size_t num_bones = m_file_data.m_skeleton_bones.size();
		new_seq->m_tracks.resize( num_bones );
		
		nASSERT( vanim.AnimInfo.TotalBones == num_bones );
		nASSERT( vanim.KeyTrack.size() == vanim.AnimInfo.NumRawFrames*num_bones);

		for(size_t t=0; t<num_bones; t++)
		{
			bone_track& track = new_seq->m_tracks[t];
			track.pos_keys.resize( new_seq->m_num_frame );
			track.rot_keys.resize( new_seq->m_num_frame );
			track.key_times.resize( new_seq->m_num_frame );

			/*
			 * ActorX的keys的排列是:
			 * for each frame:
			 *		for each bone:
			 *			add key
			*/
			for( size_t s=0; s<new_seq->m_num_frame; s++)
			{
				size_t src_index = s*num_bones+t;
				const VQuatAnimKey& src_key = vanim.KeyTrack[src_index];
				
				track.pos_keys[s] = _MAX_TO_D3D(src_key.Position);
				track.rot_keys[s] = quat_normalize( _MAX_TO_D3D(src_key.Orientation) );
				track.key_times[s] = src_key.Time/vanim.AnimInfo.AnimRate;

				if( t== 0) // root bone
				{
					/**
					*	在ActorX插件中对root bone做了特殊处理，见“int SceneIFC::DigestAnim(VActor *Thing, char* AnimName, char* RangeString )”
						if ( SerialTree[i].ParentIndex == 0 ) //#check whether root
						 {
						 // #debug #inversion
						 // Root XYZ needs to be negated to match the handedness of the reference skin, and space in Unreal in general.
						 Thing->RawAnimKeys[AnimIndex].Orientation = FQuat(-localAff.q.x,-localAff.q.y,-localAff.q.z,localAff.q.w);
						 }
				 */

					quat& rot = track.rot_keys[s];
					rot.x *= -1;
					rot.y *= -1;
					rot.z *= -1;
				}
			}
		}

		return new_seq;
	}
}//namespace nexus
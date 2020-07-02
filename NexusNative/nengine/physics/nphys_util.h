#ifndef __NEXUS_PHYSX_UTIL_H_
#define __NEXUS_PHYSX_UTIL_H_
#include "ncore.h"
#include "nphys_define.h"

namespace nexus
{
	namespace physx
	{
		// These need to be public for UnrealEd etc.
		const float nx_to_engine_scale = 100.f;
		const float engine_to_nx_scale = 0.01f;
		const float phys_skin_width = 0.00025f;
		const vector3 default_gravity = vector3(0.f,-9.8f,0.f);
	
		inline vector3 nx_extended_vector_conv(const NxExtendedVec3& nVec)
		{
			return vector3(nVec.x*nx_to_engine_scale, nVec.y*nx_to_engine_scale, nVec.z*nx_to_engine_scale);
		}

		inline NxExtendedVec3 nx_extended_vector_conv(const vector3& uVec)
		{
			return NxExtendedVec3(uVec.x*engine_to_nx_scale, uVec.y*engine_to_nx_scale, uVec.z*engine_to_nx_scale);
		}

		inline vector3 nx_vector_conv(const NxVec3& nVec)
		{
			return vector3(nVec.x*nx_to_engine_scale, nVec.y*nx_to_engine_scale, nVec.z*nx_to_engine_scale);
		}

		inline NxVec3 nx_vector_conv(const vector3& uVec)
		{
			return NxVec3(uVec.x*engine_to_nx_scale, uVec.y*engine_to_nx_scale, uVec.z*engine_to_nx_scale);
		}

		inline vector3 nx_vector_copy(const NxVec3& nVec)
		{
			return vector3(nVec.x, nVec.y, nVec.z);
		}

		inline NxVec3 nx_vector_copy(const vector3& uVec)
		{
			return NxVec3(uVec.x, uVec.y, uVec.z);
		}

		inline color4f nx_color_conv(NxU32 nColor)
		{
			float blue	= float((nColor)&0xff)/255.0f;
			float green	= float((nColor>>8)&0xff)/255.0f;
			float red	= float((nColor>>16)&0xff)/255.0f;
			float alpha	= float((nColor>>24)&0xff)/255.0f;
			return color4f(red, green, blue, alpha);
		}

		inline NxMat34 nx_matrix_conv(const matrix44& mat )
		{
			NxMat34 Result;

			// Copy rotation
			NxF32 Entries[9];
			Entries[0] = mat._11;
			Entries[1] = mat._12;
			Entries[2] = mat._13;

			Entries[3] = mat._21;
			Entries[4] = mat._22;
			Entries[5] = mat._23;

			Entries[6] = mat._31;
			Entries[7] = mat._32;
			Entries[8] = mat._33;

			Result.M.setColumnMajor(Entries);

			// Copy translation
			Result.t.x = mat._41 * engine_to_nx_scale;
			Result.t.y = mat._42 * engine_to_nx_scale;
			Result.t.z = mat._43 * engine_to_nx_scale;

			return Result;
		}

		inline matrix44 nx_matrix_conv(const NxMat34& mat )
		{
			matrix44 Result;

			// Copy rotation
			NxF32 Entries[9];
			mat.M.getColumnMajor(Entries);

			Result._11 = Entries[0];
			Result._12 = Entries[1];
			Result._13 = Entries[2];

			Result._21 = Entries[3];
			Result._22 = Entries[4];
			Result._23 = Entries[5];

			Result._31 = Entries[6];
			Result._32 = Entries[7];
			Result._33 = Entries[8];

			// Copy translation
			Result._41 = mat.t.x * nx_to_engine_scale;
			Result._42 = mat.t.y * nx_to_engine_scale;
			Result._43 = mat.t.z * nx_to_engine_scale;

			// Fix fourth column
			Result._14 = Result._24 = Result._34 = 0.0f;
			Result._44 = 1.0f;

			return Result;
		};

		// collision Filtering
		NxGroupsMask create_groups_mask(EPhysCollisionChannel channel, PhysCollisionChannelContainer* collides_channels);
		void set_actor_groupsmask(NxActor* actor, NxGroupsMask groupsmask );
		void set_actor_collisiongroup(NxActor* actor, NxCollisionGroup group);

		// physics material
		void set_actor_material(NxActor *actor, NxMaterialIndex index);

		// 设置线速度
		void set_linear_velocity_to_actor(NxActor* nActor, const NxVec3 &newVel);

		// 施加一个作用力
		void add_force_to_actor(NxActor* nActor, const NxVec3 &force, NxForceMode mode=NX_FORCE, bool bWakeUp=true);

		// 将NxConvexMesh转换为BoxShape,ref UnrealEngine3
		bool represent_convex_as_box( NxActorDesc* ActorDesc, NxConvexMesh* ConvexMesh, bool bCreateCCDSkel );
		
		// ccd 连续碰撞检测
		void make_ccd_for_sphere(NxPhysicsSDK* nx_sdk, NxSphereShapeDesc* SphereDesc);
		void make_ccd_for_box(NxPhysicsSDK* nx_sdk, NxBoxShapeDesc* BoxDesc);
		void make_ccd_for_capsule(NxPhysicsSDK* nx_sdk, NxCapsuleShapeDesc* SphylDesc);
		void make_ccd_for_convex(NxPhysicsSDK* nx_sdk, NxConvexShapeDesc* ConvexDesc);

	}
}

#endif  //__NEXUS_PHYSX_UTIL_H_
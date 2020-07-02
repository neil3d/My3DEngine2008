#include "StdAfx.h"
#include "nphys_pch.h"
#include "nphys_util.h"
#include "nlog.h"

namespace nexus
{
	namespace physx
	{
		NxGroupsMask create_groups_mask(EPhysCollisionChannel channel, PhysCollisionChannelContainer* collides_channels)
		{
			NxGroupsMask NewMask;
			memset(&NewMask,0,sizeof(NxGroupsMask));

			int ChannelShift = (int)channel;
			NewMask.bits0 = (1 << ChannelShift);

			if(collides_channels)
			{
				int offest = 0;
				NewMask.bits2 |= collides_channels->Default ? ( (++offest) << 1) : 0;
				NewMask.bits2 |= collides_channels->Nothing ? ( (++offest) << 1) : 0;
				NewMask.bits2 |= collides_channels->Pawn ? ( (++offest) << 1) : 0;
				NewMask.bits2 |= collides_channels->Vehicle ? ( (++offest) << 1) : 0;
				NewMask.bits2 |= collides_channels->Water ? ( (++offest) << 1) : 0;
				NewMask.bits2 |= collides_channels->Gameplay ? ( (++offest) << 1) : 0;
				NewMask.bits2 |= collides_channels->Particle ? ( (++offest) << 1) : 0;
				NewMask.bits2 |= collides_channels->Cloth ? ( (++offest) << 1) : 0;
				NewMask.bits2 |= collides_channels->Soft ? ( (++offest) << 1) : 0;
				NewMask.bits2 |= collides_channels->Fuild ? ( (++offest) << 1) : 0;
				NewMask.bits2 |= collides_channels->Untitled1 ? ( (++offest) << 1) : 0;
				NewMask.bits2 |= collides_channels->Untitled2 ? ( (++offest) << 1) : 0;
				NewMask.bits2 |= collides_channels->Untitled3 ? ( (++offest) << 1) : 0;
				NewMask.bits2 |= collides_channels->Untitled4 ? ( (++offest) << 1) : 0;
			}

			return NewMask;
		}

		void set_actor_groupsmask(NxActor* actor, NxGroupsMask groupsmask )
		{
			NxU32 nbShapes = actor->getNbShapes();
			NxShape*const* shapes = actor->getShapes();
			while (nbShapes--)
			{	
				NxShape* shape = shapes[nbShapes];
				shape->setGroupsMask(groupsmask);
			}
		}

		void set_actor_collisiongroup(NxActor* actor, NxCollisionGroup group)
		{
			NxShape*const* shapes = actor->getShapes();
			NxU32 nShapes = actor->getNbShapes();
			while (nShapes--)
			{
				shapes[nShapes]->setGroup(group);
			}
		}

		void set_actor_material(NxActor *actor, NxMaterialIndex index)
		{
			NxU32 nbShapes = actor->getNbShapes();
			NxShape*const* shapes = actor->getShapes();

			while (nbShapes--)
			{
				shapes[nbShapes]->setMaterial(index);
			}
		}

		void set_linear_velocity_to_actor(NxActor* nActor, const NxVec3 &newVel)
		{
		#if _DEBUG && NPHYS_DEBUG
			if(newVel.magnitude() > NPHYS_DEBUG_MAGNITUDE_THRESHOLD)
			{
				nLog_Warning( _T("Setting Linear Velocity to a large magnitude. Setting to: %f %f %f"),newVel[0],newVel[1],newVel[2]);
			}
		#endif

			nActor->setLinearVelocity(newVel);
		}

		void add_force_to_actor(NxActor* nActor, const NxVec3 &force, NxForceMode mode/*=NX_FORCE*/, bool bWakeUp/*=true*/)
		{
		#if _DEBUG && NPHYS_DEBUG
			if(force.magnitude() > NPHYS_DEBUG_MAGNITUDE_THRESHOLD )
			{
				nLog_Warning( _T("Adding a large force ( %f %f %f ) to an object."),force[0],force[1],force[2]);
			}
		#endif

			nActor->addForce(force,mode,bWakeUp);
		}

		const NxVec3 * get_convexmesh_vertex( NxConvexMeshDesc* desc, NxU32 index )
		{
			return (const NxVec3 *)((NxU8*)desc->points + index*desc->pointStrideBytes);
		}

		void get_convexmesh_triangle( NxConvexMeshDesc* desc, NxU32 index, const NxVec3* tri[3] )
		{
			NxU32 c = (desc->flags & NX_CF_FLIPNORMALS) != 0;
			if( desc->flags & NX_CF_16_BIT_INDICES )
			{
				const NxU16 * trig16 = (const NxU16 *)((NxU8*)desc->triangles + index*desc->triangleStrideBytes);
				tri[0] = get_convexmesh_vertex( desc, trig16[0] );
				tri[1] = get_convexmesh_vertex( desc, trig16[1+c] );
				tri[2] = get_convexmesh_vertex( desc, trig16[2-c] );
			}
			else
			{
				const NxU32 * trig32 = (const NxU32 *)((NxU8*)desc->triangles + index*desc->triangleStrideBytes);
				tri[0] = get_convexmesh_vertex( desc, trig32[0] );
				tri[1] = get_convexmesh_vertex( desc, trig32[1+c] );
				tri[2] = get_convexmesh_vertex( desc, trig32[2-c] );
			}
		}

		bool represent_convex_as_box( NxActorDesc* ActorDesc, NxConvexMesh* ConvexMesh, bool bCreateCCDSkel )
		{
			const NxReal eps = (NxReal)0.001;
			const NxReal oneMinusHalfEps = (NxReal)1 - (NxReal)0.5*eps*eps;

			NxConvexMeshDesc desc;
			desc.numVertices         = ConvexMesh->getCount(0,  NX_ARRAY_VERTICES);
			desc.numTriangles        = ConvexMesh->getCount(0,  NX_ARRAY_TRIANGLES);
			desc.pointStrideBytes    = ConvexMesh->getStride(0, NX_ARRAY_VERTICES);
			desc.triangleStrideBytes = ConvexMesh->getStride(0, NX_ARRAY_TRIANGLES);
			desc.points              = ConvexMesh->getBase(0,   NX_ARRAY_VERTICES);
			desc.triangles           = ConvexMesh->getBase(0,   NX_ARRAY_TRIANGLES);
			desc.flags               = ConvexMesh->getFormat(0, NX_ARRAY_TRIANGLES)==NX_FORMAT_SHORT ? NX_CF_16_BIT_INDICES : 0;

			// A box should have eight vertices and twelve triangles
			if( desc.numVertices != 8 || desc.numTriangles != 12 )
			{
				return false;
			}

			int NormalsFound = 0;
			// Normals should bin into three groups, if we ignore overall factor of (-1)
			NxVec3 normal[3];
			int NormalCount[3] = {0,0,0};	// Reality nASSERT, should find two of each
			int AntiNormalCount[3] = {0,0,0};	// Reality nASSERT, should find two of each
			NxReal radii[3] = {0,0,0};

			int i;
			for( i = 0; i < 12; ++i )
			{
				// Get the i(th) triangle and its normal
				const NxVec3* tri[3];
				get_convexmesh_triangle( &desc, i, tri );
				NxVec3 n;
				n.cross( *(tri[1]) - *(tri[0]), *(tri[2]) - *(tri[0]) );
				n.normalize();

				// See if it matches any of the previously found normals
				int j;
				for( j = 0; j < NormalsFound; ++j )
				{
					NxReal proj = n.dot( normal[j] );
					if( proj > oneMinusHalfEps )
					{
						++NormalCount[j];
						break;
					}
					if( proj < -oneMinusHalfEps )
					{
						if( !AntiNormalCount[j] )
						{	// Take this as an opportunity to calculate the box extent in this direction
							radii[j] += n.dot( *(tri[0]) );
							radii[j] = fabsf( (NxReal)0.5*radii[j] );
						}
						++AntiNormalCount[j];
						break;
					}
				}
				if( j == NormalsFound )
				{
					if( NormalsFound >= 3 )
					{
						return false;	// More than three normals, can't be a cube
					}
					// New normal - add it
					normal[NormalsFound++] = n;
					radii[j] = n.dot( *(tri[0]) );	// This is a temporary value
					++NormalCount[j];
				}
			}

			// Should have three distinct normals
			if( NormalsFound != 3 )
			{
				return false;
			}

			// Triangle normals should have come in pairs
			if( NormalCount[0] != 2 || NormalCount[1] != 2 || NormalCount[2] != 2 )
			{
				return false;
			}

			// And corresponding triangles on the opposite faces
			if( AntiNormalCount[0] != 2 || AntiNormalCount[1] != 2 || AntiNormalCount[2] != 2 )
			{
				return false;
			}

			// Make sure normals are orthogonal
			if( abs(normal[0].dot(normal[1])) > eps ||
				abs(normal[1].dot(normal[2])) > eps ||
				abs(normal[2].dot(normal[0])) > eps )
			{
				return false;
			}

			// Finally, make sure we have a right-handed coordinate system
			if( (normal[0].cross( normal[1] )).dot( normal[2] ) < 0 )
			{
				normal[2] *= -(NxReal)1;
			}

			// We have a box!  Find its center
			NxVec3 center( (NxReal)0, (NxReal)0, (NxReal)0 );
			for( i = 0; i < 8; ++i )
			{
				center += *get_convexmesh_vertex( &desc, i );
			}
			center *= (NxReal)0.125;

			// The three normal directions and center form the rows of the relative TM
			NxMat33 rot( normal[0], normal[1], normal[2] );
			rot.setTransposed( rot );
			NxMat34 RelativeTM( rot, center );

			// Create the box description
			NxBoxShapeDesc* BoxDesc = new NxBoxShapeDesc;
			BoxDesc->dimensions = NxVec3( radii[0], radii[1], radii[2] );
			BoxDesc->localPose = RelativeTM;
			if(bCreateCCDSkel)
			{
				//make_ccd_for_box(BoxDesc);
			}

			ActorDesc->shapes.pushBack(BoxDesc);

			return true;
		}

		static const float CCDScaleAmount = 0.5f;
		static const float RecipSqrtOfThree = 0.5773f;

		static void add_triangle(std::vector<int>& InIndexBuffer, int Index0, int Index1, int Index2)
		{
			InIndexBuffer.push_back(Index0);
			InIndexBuffer.push_back(Index1);
			InIndexBuffer.push_back(Index2);
		}

		void make_ccd_for_sphere(NxPhysicsSDK* nx_sdk, NxSphereShapeDesc* SphereDesc)
		{
			if(!nx_sdk)
			{
				return;
			}

			std::vector<vector3>	tetra_verts;

			// Distance from origin to each vert is sqrt(3). 
			// So to make a tetrahedron whose pointer are (Radius * CCDShrinkAmount) away from origin, we divide by sqrt(3).
			tetra_verts.push_back( vector3(1,1,1) * (float)SphereDesc->radius * CCDScaleAmount * RecipSqrtOfThree );
			tetra_verts.push_back( vector3(-1,-1,1) * (float)SphereDesc->radius * CCDScaleAmount * RecipSqrtOfThree );
			tetra_verts.push_back( vector3(-1,1,-1) * (float)SphereDesc->radius * CCDScaleAmount * RecipSqrtOfThree );
			tetra_verts.push_back( vector3(1,-1,-1) * (float)SphereDesc->radius * CCDScaleAmount * RecipSqrtOfThree );

			// Make index buffer
			std::vector<int> triangle_indices;

			add_triangle(triangle_indices, 0, 2, 1);
			add_triangle(triangle_indices, 0, 1, 3);
			add_triangle(triangle_indices, 0, 3, 2);
			add_triangle(triangle_indices, 1, 2, 3);

			NxSimpleTriangleMesh nTriMesh;
			nTriMesh.points = &(*tetra_verts.begin());
			nTriMesh.numVertices = 4;
			nTriMesh.pointStrideBytes = sizeof(vector3);
			nTriMesh.numTriangles = 4;
			nTriMesh.triangles = &(*triangle_indices.begin());
			nTriMesh.triangleStrideBytes = sizeof(int) * 3;
			nTriMesh.flags = 0;

			SphereDesc->ccdSkeleton = nx_sdk->createCCDSkeleton(nTriMesh);
		}

		void make_ccd_for_box(NxPhysicsSDK* nx_sdk, NxBoxShapeDesc* BoxDesc)
		{
			if(!nx_sdk)
			{
				return;
			}

			std::vector<vector3>	BoxVerts;
			vector3 SkelRadii = vector3(BoxDesc->dimensions.x, BoxDesc->dimensions.y, BoxDesc->dimensions.z) *  CCDScaleAmount;

			BoxVerts.push_back( vector3(-1,-1,-1)*SkelRadii );
			BoxVerts.push_back( vector3( 1,-1,-1)*SkelRadii );
			BoxVerts.push_back( vector3( 1, 1,-1)*SkelRadii );
			BoxVerts.push_back( vector3(-1, 1,-1)*SkelRadii );
												 
			BoxVerts.push_back( vector3(-1,-1, 1)*SkelRadii );
			BoxVerts.push_back( vector3( 1,-1, 1)*SkelRadii );
			BoxVerts.push_back( vector3( 1, 1, 1)*SkelRadii );
			BoxVerts.push_back( vector3(-1, 1, 1)*SkelRadii );

			// Make index buffer
			std::vector<int> BoxIndices;

			add_triangle(BoxIndices, 0, 2, 1);
			add_triangle(BoxIndices, 0, 3, 2);

			add_triangle(BoxIndices, 1, 6, 5);
			add_triangle(BoxIndices, 1, 2, 6);

			add_triangle(BoxIndices, 5, 7, 4);
			add_triangle(BoxIndices, 5, 6, 7);

			add_triangle(BoxIndices, 4, 3, 0);
			add_triangle(BoxIndices, 4, 7, 3);

			add_triangle(BoxIndices, 3, 6, 2);
			add_triangle(BoxIndices, 3, 7, 6);

			add_triangle(BoxIndices, 5, 0, 1);
			add_triangle(BoxIndices, 5, 4, 0);

			NxSimpleTriangleMesh nTriMesh;
			nTriMesh.points = &(*BoxVerts.begin());
			nTriMesh.numVertices = 8;
			nTriMesh.pointStrideBytes = sizeof(vector3);
			nTriMesh.numTriangles = 12;
			nTriMesh.triangles = &(*BoxIndices.begin());
			nTriMesh.triangleStrideBytes = sizeof(int) * 3;
			nTriMesh.flags = 0;

			BoxDesc->ccdSkeleton = nx_sdk->createCCDSkeleton(nTriMesh);
		}

		void make_ccd_for_capsule(NxPhysicsSDK* nx_sdk, NxCapsuleShapeDesc* CapsuleDesc)
		{
			if(!nx_sdk)
			{
				return;
			}

			std::vector<vector3>	PrismVerts;
			// Main axis is Y. Verts for triangle at one end
			PrismVerts.push_back((vector3(0, 1, 0) * 0.5f * CapsuleDesc->height) + (vector3(1,0,0) * CapsuleDesc->radius * CCDScaleAmount));
			PrismVerts.push_back((vector3(0, 1, 0) * 0.5f * CapsuleDesc->height) + (vector3(-0.5f,0,0.866025f) * CapsuleDesc->radius * CCDScaleAmount));
			PrismVerts.push_back((vector3(0, 1, 0) * 0.5f * CapsuleDesc->height) + (vector3(-0.5f,0,-0.866025f) * CapsuleDesc->radius * CCDScaleAmount));
								  
			// Triangle at the other end
			PrismVerts.push_back((vector3(0, 1, 0) * -0.5f * CapsuleDesc->height) + (vector3(1,0,0) * CapsuleDesc->radius * CCDScaleAmount));
			PrismVerts.push_back((vector3(0, 1, 0) * -0.5f * CapsuleDesc->height) + (vector3(-0.5f,0,0.866025f) * CapsuleDesc->radius * CCDScaleAmount));
			PrismVerts.push_back((vector3(0, 1, 0) * -0.5f * CapsuleDesc->height) + (vector3(-0.5f,0,-0.866025f) * CapsuleDesc->radius * CCDScaleAmount));

			// Make index buffer
			std::vector<int> PrismIndices;

			// Four rectangular faces
			add_triangle(PrismIndices, 0, 4, 3);
			add_triangle(PrismIndices, 0, 1, 4);

			add_triangle(PrismIndices, 1, 5, 4);
			add_triangle(PrismIndices, 1, 2, 5);

			add_triangle(PrismIndices, 2, 3, 5);
			add_triangle(PrismIndices, 2, 0, 3);

			// End triangles
			add_triangle(PrismIndices, 0, 2, 1);
			add_triangle(PrismIndices, 5, 3, 4);

			NxSimpleTriangleMesh nTriMesh;
			nTriMesh.points = &(*PrismVerts.begin());
			nTriMesh.numVertices = 6;
			nTriMesh.pointStrideBytes = sizeof(vector3);
			nTriMesh.numTriangles = 8;
			nTriMesh.triangles = &(*PrismIndices.begin());
			nTriMesh.triangleStrideBytes = sizeof(int) * 3;
			nTriMesh.flags = 0;

			CapsuleDesc->ccdSkeleton = nx_sdk->createCCDSkeleton(nTriMesh);
		}

		void make_ccd_for_convex(NxPhysicsSDK* nx_sdk, NxConvexShapeDesc* ConvexDesc)
		{
			if(!nx_sdk)
			{
				return;
			}

			NxConvexMesh* Mesh = ConvexDesc->meshData;

			// Extract vertex info from mesh
			int VertCount = Mesh->getCount(0,NX_ARRAY_VERTICES);    

			const void* VertBase = Mesh->getBase(0,NX_ARRAY_VERTICES);    

			NxU32 VertStride = Mesh->getStride(0,NX_ARRAY_VERTICES);
			nASSERT(VertStride == sizeof(vector3));

			// Copy verts out, whilst calculating centroid.
			std::vector<vector3>	ConvexVerts;
			vector3 Centroid(0,0,0);
			for(int i=0; i<VertCount; i++)
			{
				// Copy vertex from the mesh
				vector3* V = ((vector3*)VertBase) + i;
				ConvexVerts.push_back( *V );

				// Accumulate vertex positions.
				Centroid += ConvexVerts[i];
			}
			Centroid = Centroid / (float)VertCount;

			// New smush verts back towards centroid.
			for(int i=0; i<VertCount; i++)
			{
				vector3 Delta = ConvexVerts[i] - Centroid;
				ConvexVerts[i] = Centroid + ( Delta * CCDScaleAmount );
			}

			// Get index information
			int TriCount = Mesh->getCount(0,NX_ARRAY_TRIANGLES);
			nASSERT(TriCount > 0);

			NxInternalFormat IndexFormat = Mesh->getFormat(0,NX_ARRAY_TRIANGLES);
			nASSERT(IndexFormat == NX_FORMAT_INT);

			int* IndexBase = (int*)(Mesh->getBase(0,NX_ARRAY_TRIANGLES));

			NxU32 IndexStride = Mesh->getStride(0,NX_ARRAY_TRIANGLES);
			nASSERT(IndexStride == sizeof(int) * 3);

			// Copy indices out to make index buffer.
			std::vector<int>	ConvexIndices;
			for(int i=0; i<TriCount; i++)
			{
				ConvexIndices.push_back( IndexBase[(i*3)+0] );
				ConvexIndices.push_back( IndexBase[(i*3)+1] );
				ConvexIndices.push_back( IndexBase[(i*3)+2] );
			}

			NxSimpleTriangleMesh nTriMesh;
			nTriMesh.points = &(*ConvexVerts.begin());
			nTriMesh.numVertices = VertCount;
			nTriMesh.pointStrideBytes = sizeof(vector3);
			nTriMesh.numTriangles = TriCount;
			nTriMesh.triangles = &(*ConvexIndices.begin());
			nTriMesh.triangleStrideBytes = sizeof(int) * 3;
			nTriMesh.flags = 0;

			ConvexDesc->ccdSkeleton = nx_sdk->createCCDSkeleton(nTriMesh);
		}


	} // end of namespace physx
} // end of namespace nexus

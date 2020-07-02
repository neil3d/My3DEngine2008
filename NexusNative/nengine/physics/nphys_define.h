/**
*	Nexus Engine
*
*	Author: zewoo
*	Date:	Aug, 2009
*/

#ifndef _NPHYS_DEFINE_H_
#define _NPHYS_DEFINE_H_

enum EPhysicsState
{
	Phys_NULL               =0,
	Phys_Walking            =1,
	Phys_Falling            =2,
	Phys_Swimming           =3,
	Phys_Flying             =4,
	Phys_Rotating           =5,
	Phys_Projectile         =6,
	Phys_Interpolating      =7,
	Phys_Spider             =8,
	Phys_Ladder             =9,
	Phys_RigidBody          =10,
	Phys_SoftBody           =11,
	Phys_MAX
};

enum EPhysCollisionChannel
{
	PhysCC_Default,
	PhysCC_Nothing,
	PhysCC_Pawn,
	PhysCC_Vehicle,
	PhysCC_Water,
	PhysCC_Gameplay,
	PhysCC_Particle,
	PhysCC_Cloth,
	PhysCC_Soft,
	PhysCC_Fuild,
	PhysCC_Untitled1,
	PhysCC_Untitled2,
	PhysCC_Untitled3,
	PhysCC_Untitled4,
};

struct PhysCollisionChannelContainer
{
	bool		Default;
	bool		Nothing;
	bool		Pawn;
	bool		Vehicle;
	bool		Water;
	bool		Gameplay;
	bool		Particle;
	bool		Cloth;
	bool		Soft;
	bool		Fuild;
	bool		Untitled1;
	bool		Untitled2;
	bool		Untitled3;
	bool		Untitled4;

	PhysCollisionChannelContainer()
	{
		memset( this, 0, sizeof(*this) );
	}
};


#endif
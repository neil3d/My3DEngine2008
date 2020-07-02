#ifndef __NEXUS_PHYS_PCH_H_
#define __NEXUS_PHYS_PCH_H_

#include "NxPhysics.h"
#include "NxStream.h"
#include "NxCooking.h"
#include "NxControllerManager.h"
#include "NxBoxController.h"
#include "NxCapsuleController.h"
#include "NxDebugRenderable.h" 

#pragma comment(lib, "NxCharacter.lib")
#pragma comment(lib, "PhysXCore.lib")
#pragma comment(lib, "PhysXCooking.lib")
#pragma comment(lib, "PhysXLoader.lib")

#define NPHYS_DEBUG 1
#define NPHYS_DEBUG_MAGNITUDE_THRESHOLD 10000

#endif
#ifndef _NEXUS_SPEED_TREE_SDK_CONFIG_H_
#define _NEXUS_SPEED_TREE_SDK_CONFIG_H_

#define SPEEDTREE_UPVECTOR_Y
#define SPEEDTREE_BRANCH_DETAIL_LAYER
#define SPEEDTREE_BRANCH_NORMAL_MAPPING
#define SPEEDTREE_LEAF_NORMAL_MAPPING
#define SPEEDTREE_FROND_NORMAL_MAPPING
#define SPEEDTREE_BRANCH_FADING

#include "../SPT_SDK/Include/SpeedTreeRT.h"

///////////////////////////////////////////////////////////////////////  
//  Constants
//
//  These two constants are used to override the corresponding values in the SpeedWind
//  .ini files.  The line in the .ini file normally looks like this:
//
//      NumWindMatricesAndLeafAngles 6 8

const int c_nNumWindMatricesPerSource = 6;
const int c_nNumLeafAnglesPerSource = 8;

#endif //_NEXUS_SPEED_TREE_SDK_CONFIG_H_
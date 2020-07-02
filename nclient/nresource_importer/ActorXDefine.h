#pragma once

using namespace nexus;
#pragma pack( push, 4)

//
// File header structure. 
//
struct VChunkHdr
{
	char		ChunkID[20];  // string ID of up to 19 chars (usually zero-terminated?)
	int			TypeFlag;     // Flags/reserved/Version number...
	int         DataSize;     // size per struct following;
	int         DataCount;    // number of structs/
};

//
// Point
//
struct VPoint
{	
	vector3		Point;
};

//
// Vertex with texturing info, akin to Hoppe's 'Wedge' concept.
//
struct VVertex
{
	WORD	PointIndex;	 // Index to a point.
	float   U,V;         // Engine may choose to store these as floats, words,bytes - but raw PSK file has full floats.
	BYTE    MatIndex;    // At runtime, this one will be implied by the vertex that's pointing to us.
	BYTE    Reserved;    // Top secret.
};

//
// Textured triangle.
//
struct VTriangle
{
	WORD    WedgeIndex[3];	 // point to three vertices in the vertex list.
	BYTE    MatIndex;	     // Materials can be anything.
	BYTE    AuxMatIndex;     // Second material (eg. damage skin, shininess, detail texture / detail mesh...
	DWORD   SmoothingGroups; // 32-bit flag for smoothing groups AND Lod-bias calculation.
};

//
// A Material
//
struct VMaterial
{
	char		MaterialName[64]; // Straightforward ascii array, for binary input.
	int         TextureIndex;     // multi/sub texture index 
	DWORD		PolyFlags;        // all poly's with THIS material will have this flag.
	int         AuxMaterial;      // index into another material, eg. alpha/detailtexture/shininess/whatever
	DWORD		AuxFlags;		  // reserved: auxiliary flags 
	INT			LodBias;          // material-specific lod bias
	INT			LodStyle;         // material-specific lod style
};

// A bone: an orientation, and a position, all relative to their parent.
struct VJointPos
{
	quat	 	Orientation;  //
	vector3		Position;     //  needed or not ?

	float       Length;       //  For collision testing / debugging drawing...
	float       XSize;	      //
	float       YSize;        //
	float       ZSize;        //
};

struct FNamedBoneBinary
{
	char	   Name[64];     // ANSICHAR   Name[64];	// Bone's name
	DWORD      Flags;		 // reserved
	INT        NumChildren;  //
	INT		   ParentIndex;	 // 0/NULL if this is the root bone.  
	VJointPos  BonePos;	     //
};

struct VRawBoneInfluence // Just weight, vertex, and Bone, sorted later.
{
	float Weight;
	INT   PointIndex;
	INT   BoneIndex;
};


//-- animation -------------------------------------------------
struct VAnimInfoBinary
{
	char	Name[64];     // Animation's name
	char	Group[64];    // Animation's group name	

	INT		TotalBones;           // TotalBones * NumRawFrames is number of animation keys to digest.

	INT		ScaleInclude;         // if 1, scaling keys will follow in the scale key chunk for this sequence.  Old name: "RootInclude".
	INT		KeyCompressionStyle;  // Reserved: variants in tradeoffs for compression.
	INT		KeyQuotum;            // Max key quotum for compression	
	FLOAT	KeyReduction;       // desired 
	FLOAT	TrackTime;          // explicit - can be overridden by the animation rate
	FLOAT	AnimRate;           // frames per second.
	INT		StartBone;            // - Reserved: for partial animations.
	INT		FirstRawFrame;        //
	INT		NumRawFrames;         //
};

struct VQuatAnimKey
{
	vector3		Position;           // relative to parent.
	quat		Orientation;        // relative to parent.
	FLOAT       Time;				// The duration until the next key (end key wraps to first...)
};

struct VScaleAnimKey
{	
	vector3 ScaleVector;   // If uniform scaling is required, just use the X component..
	FLOAT   Time;          // disregarded	
};

#pragma pack( pop )

//
// Complete Animation for a skeleton.
// Independent-sized key tracks for each part of the skeleton.
//
struct VAnimation
{
	VAnimInfoBinary		       AnimInfo;   //  Generic animation info as saved for the engine
	std::vector<VQuatAnimKey>  KeyTrack;   //  Animation track - keys * bones
	std::vector<VScaleAnimKey> ScaleTrack; //  Animated scaling track - keys * bones
};
#ifndef MODELJONTS_H
#define MODELJONTS_H

typedef float vec3_t[3];

typedef struct Placement_s
{
	vec3_t origin;
	vec3_t direction;
	vec3_t up;
} Placement_t;

typedef struct ModelJointListNode_s
{
	struct ModelJoint_s *joint;
	struct ModelJointListNode_s *next;
} ModelJointListNode_t;

typedef struct ModelJoint_s
{
	Placement_t model;	// relative to the model, used for dynamic software rotation
	Placement_t parent;	// relative to the parent joint (or model in case of root joint), used for
						// inverse kinematics
	ModelJointListNode_t *children;
} ModelJoint_t;

typedef struct
{
	int children;		// must be the first field
	int numVerticies;
	int *verticies;
	int inUse;
} ModelCluster_t;

#define MAX_MODELJOINTS			255
#define MAX_MODELJOINTNODES		254

extern ModelCluster_t modelClusters[MAX_MODELJOINTS];

extern struct ArrayedListNode_s modelJointNodes[MAX_MODELJOINTNODES];

extern void *Hunk_Alloc (int size);

void CreateFrameSkeleton(ModelJoint_t *baseJoint, int structure);

#endif
//***************************************************************************************
// PhysX.h
//
//
// 
//
//***************************************************************************************
#ifndef PHYSX_H
#define PHYSX_H
#include < PxPhysicsAPI.h >
#include < extensions/PxDefaultErrorCallback.h >
#include < extensions/PxDefaultAllocator.h > 
#include < PxToolkit.h >
#include "PhysXParticles.h"

#include <vector>

using namespace std;
using namespace physx;

#pragma comment(lib, "PhysX3_x86.lib")
#pragma comment(lib, "PhysX3Common_x86.lib") 
#pragma comment(lib, "PhysX3Cooking_x86.lib") 
#pragma comment(lib, "PhysX3Extensions.lib")

#define MAX_BOXES 1000
#define MAX_PARTICLES 100;

enum ObjectNumbers{
	block = 0,
	tree = 1,
	cow = 2,
	terrain = 3,
	Last = 4
};

struct TriMeshObj{
	PxTriangleMeshDesc sMeshDesc;
	ObjectNumbers sObjectNumber;
};

class PhysX
{
public:
    PhysX();
    ~PhysX();
    void Init();
	bool advance(float dt);
    void fetch();

	void CreateTerrain( int numVerts, PxVec3* verts, int numInds, int* inds);

	void SetupTriangleMesh(	ObjectNumbers objnum, int numVerts, PxVec3* verts,
							int numInds, int* inds, float x, float y, float z, float scale);

	void PlaceTriangleMesh( ObjectNumbers objnum, float x, float y, float z, float scale, bool statc );
	
	void CreateBox(float x, float y, float z, float vx, float vy, float vz, float speed);
	PxTransform GetBoxWorld(int boxnum);
    int GetNumBoxes();

	void InitParticles(int count, float x, float y, float z, float vx, float vy, float vz, bool gravity);

public:
    PxFoundation*					mFoundation;
    PxPhysics*						mPhysics;
	PxScene*						mScene;
	
	PxMaterial*						mMaterial;
	PxDefaultCpuDispatcher*			mCpuDispatcher;
	//pxtask::CudaContextManager*	mCudaContextManager;

	PxU32							mNbThreads;

	
	//std::vector<PhysXParticles>  mParticles;
	

	//std::vector<PxRigidActor*>				mPhysicsActors;
};

#endif
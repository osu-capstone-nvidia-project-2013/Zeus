//***************************************************************************************
// PhysX.h
//
//
// 
//
//***************************************************************************************
#ifndef PHYSX_H
#define PHYSX_H
#include <PxPhysicsAPI.h>
#include < extensions/PxDefaultErrorCallback.h >
#include < extensions/PxDefaultAllocator.h > 



using namespace physx;

#pragma comment(lib, "PhysX3_x86.lib")
#pragma comment(lib, "PhysX3Common_x86.lib") 
#pragma comment(lib, "PhysX3Cooking_x86.lib") 
#pragma comment(lib, "PhysX3Extensions.lib")

#define MAX_BOXES 1000

enum ObjectNumbers{
	block,
	tree,
	cow,
	terrain
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

	void SetupTriangleMesh(	ObjectNumbers objnum, int numVerts, PxVec3* verts,
							int numInds, int* inds);

	void CreateSphere(float x, float y, float z);
	void CreateBox(float x, float y, float z, float vx, float vy, float vz, float speed);
	PxTransform GetSphereWorld();
	PxTransform GetBoxWorld(int boxnum);
    int GetNumBoxes();

	void Draw(PxTransform &transform);

public:
    PxFoundation*           mFoundation;
    PxPhysics*              mPhysics;
	PxScene*				mScene;
	
	PxMaterial*								mMaterial;
	PxDefaultCpuDispatcher*					mCpuDispatcher;
	//pxtask::CudaContextManager*				mCudaContextManager;

	PxU32									mNbThreads;

	

	

	//std::vector<PxRigidActor*>				mPhysicsActors;
};

#endif
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
#pragma comment(lib, "PhysX3Extensions.lib")

class PhysX
{
public:
    PhysX();
    ~PhysX();
    void Init();

private:
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
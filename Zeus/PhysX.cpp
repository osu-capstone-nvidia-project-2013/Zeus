
//***************************************************************************************
// PhysX.cpp
//
//
// 
//
//***************************************************************************************

#include "PhysX.h"

PhysX::PhysX() :
    mFoundation(NULL),
    mPhysics(NULL),
	mScene(NULL),
	mMaterial(NULL),
	mCpuDispatcher(NULL),
	//mCudaContextManager(NULL),
	mNbThreads(1)
{
}

PhysX::~PhysX()
{
    mPhysics->release();
    mFoundation->release();
	mScene->release();
	mCpuDispatcher->release();
	mMaterial->release();
}

void PhysX::Init()
{
    static PxDefaultErrorCallback gDefaultErrorCallback;
    static PxDefaultAllocator	  gDefaultAllocatorCallback;
   /* mFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, gDefaultAllocatorCallback, gDefaultErrorCallback);
	if(!mFoundation)
		return;

    bool recordMemoryAllocations = true;*/

 //   mPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *mFoundation, PxTolerancesScale(), recordMemoryAllocations, NULL);

 //   PxInitExtensions(*mPhysics);

	//PxSceneDesc sceneDesc(mPhysics->getTolerancesScale());
	//sceneDesc.gravity = PxVec3(0.0f, -9.81f, 0.0f);
	////customizeSceneDesc(sceneDesc);

	//mCpuDispatcher = PxDefaultCpuDispatcherCreate(mNbThreads);
	//sceneDesc.cpuDispatcher	= mCpuDispatcher;

	////sceneDesc.gpuDispatcher = mCudaContextManager->getGpuDispatcher();
	//
	//sceneDesc.filterShader	= PxDefaultSimulationFilterShader;

	//mScene = mPhysics->createScene(sceneDesc);
}
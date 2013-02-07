
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
	
	PxFoundation*				pxFoundation;
	PxScene*					pxScene;
	PxPhysics*					pxPhysics;
	PxDefaultCpuDispatcher*		pxCpuDispatcher;
	PxMaterial*					pxMaterial;
	PxCooking*					pxCooking;

	struct TriMeshObj*			objectsLoaded;
	
PhysX::~PhysX()
{
    mPhysics->release();
    mFoundation->release();
	mScene->release();
	mCpuDispatcher->release();
	mMaterial->release();

	pxFoundation->release();
	pxScene->release();
	pxPhysics->release();
	pxCpuDispatcher->release();
	pxMaterial->release();
}

void PhysX::Init()
{
    static PxDefaultErrorCallback gDefaultErrorCallback;
    static PxDefaultAllocator	  gDefaultAllocatorCallback;

	pxFoundation = NULL;
	//PxCreateFoundation(PX_PHYSICS_VERSION, gDefaultAllocatorCallback, gDefaultErrorCallback);
	pxFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, gDefaultAllocatorCallback, gDefaultErrorCallback);

    /*mFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, gDefaultAllocatorCallback, gDefaultErrorCallback);
	if(!mFoundation)
		return;*/

    bool recordMemoryAllocations = true;
	
	pxPhysics = NULL;
	//PxCreatePhysics(PX_PHYSICS_VERSION, *pxfoundation, PxTolerancesScale(), recordMemoryAllocations, NULL);
    pxPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *pxFoundation, PxTolerancesScale(), recordMemoryAllocations, NULL);

    PxInitExtensions(*pxPhysics);

    
	PxSceneDesc sceneDesc(pxPhysics->getTolerancesScale());
	sceneDesc.gravity = PxVec3(0.0f, -9.81f, 0.0f);
	//customizeSceneDesc(sceneDesc);


	pxCpuDispatcher = PxDefaultCpuDispatcherCreate(1);
	sceneDesc.cpuDispatcher	= pxCpuDispatcher;

	//sceneDesc.gpuDispatcher = mCudaContextManager->getGpuDispatcher();
	
	sceneDesc.filterShader	= PxDefaultSimulationFilterShader;

	pxScene = pxPhysics->createScene(sceneDesc);

	pxMaterial = pxPhysics->createMaterial(0.5f, 0.5f, 0.1f);    //static friction, dynamic friction, restitution
	if(!pxMaterial)
		return;

	pxCooking = PxCreateCooking(PX_PHYSICS_VERSION, *pxFoundation, PxCookingParams());
	if(!pxCooking)
		return;

	objectsLoaded = new TriMeshObj[ObjectNumbers::Last];

	PxRigidStatic* plane = PxCreatePlane(*pxPhysics, PxPlane(PxVec3(0,1,0), 0), *pxMaterial);
	if (!plane)
		return;
	
	pxScene->addActor(*plane);
	
}

float mAccumulator = 0.0f;
float mStepSize = 1.0f / 120.0f;
float mCooldown = 0.0f;

bool PhysX::advance(float dt)
{
	mAccumulator  += dt;
    if(mAccumulator < mStepSize)
        return false;

    mAccumulator -= mStepSize;

	if(mCooldown > 0.0f)
		mCooldown -= mStepSize;

    pxScene->simulate(mStepSize);
    return true;
}

void PhysX::fetch()
{
    pxScene->fetchResults(true);
}

void PhysX::CreateTerrain( int numVerts, PxVec3* verts, int numInds, int* inds)
{
	PxRigidStatic* meshActor = pxPhysics->createRigidStatic(PxTransform::createIdentity());
	PxShape* meshShape;
	if(meshActor)
	{
			
			PxTriangleMeshDesc meshDesc;
			meshDesc.points.count           = numVerts;
			meshDesc.points.stride          = sizeof(PxVec3);
			meshDesc.points.data            = verts;

			meshDesc.triangles.count        = numInds/3.;
			meshDesc.triangles.stride       = 3*sizeof(int);
			meshDesc.triangles.data         = inds;

			PxToolkit::MemoryOutputStream writeBuffer;
			bool status = pxCooking->cookTriangleMesh(meshDesc, writeBuffer);
			if(!status)
				return;

			PxToolkit::MemoryInputData readBuffer(writeBuffer.getData(), writeBuffer.getSize());
			
			PxTriangleMeshGeometry triGeom;
			triGeom.triangleMesh = pxPhysics->createTriangleMesh(readBuffer);

			meshShape = meshActor->createShape(triGeom, *pxMaterial);

			pxScene->addActor(*meshActor);
	}
}

int nv,ni;
int* indes;
PxVec3* vertas;

void PhysX::SetupTriangleMesh(	ObjectNumbers objnum, int numVerts, PxVec3* verts,
							int numInds, int* inds, float x, float y, float z)
{
	PxRigidStatic* meshActor = pxPhysics->createRigidStatic(PxTransform::createIdentity());
	PxShape* meshShape;
	if(meshActor)
	{
			

			PxTriangleMeshDesc meshDesc;
			meshDesc.points.count           = numVerts;
			meshDesc.points.stride          = sizeof(PxVec3);
			meshDesc.points.data            = verts;

			meshDesc.triangles.count        = numInds/3.;
			meshDesc.triangles.stride       = 3*sizeof(int);
			meshDesc.triangles.data         = inds;

			PxToolkit::MemoryOutputStream writeBuffer;
			bool status = pxCooking->cookTriangleMesh(meshDesc, writeBuffer);
			if(!status)
				return;

			PxToolkit::MemoryInputData readBuffer(writeBuffer.getData(), writeBuffer.getSize());
			
			PxTriangleMeshGeometry triGeom;
			triGeom.triangleMesh = pxPhysics->createTriangleMesh(readBuffer);
			triGeom.scale = PxMeshScale(PxVec3(8.0,8.0,8.0),PxQuat());

			meshShape = meshActor->createShape(triGeom, *pxMaterial);
			meshShape->setLocalPose(PxTransform(PxVec3(x,y,z)));

			pxScene->addActor(*meshActor);

			nv = numVerts;
			ni = numInds;
			indes = inds;
			vertas = verts;
	}
}

void PhysX::PlaceTriangleMesh( ObjectNumbers objnum, float x, float y, float z, float scale, bool statc )
{
	PxRigidDynamic* meshActor = pxPhysics->createRigidDynamic(PxTransform::createIdentity());
	PxShape* meshShape;
	if(meshActor)
	{
			meshActor->setRigidDynamicFlag(PxRigidDynamicFlag::eKINEMATIC, true);

			PxTriangleMeshDesc meshDesc;
			meshDesc.points.count           = nv;
			meshDesc.points.stride          = sizeof(PxVec3);
			meshDesc.points.data            = vertas;

			meshDesc.triangles.count        = ni/3.;
			meshDesc.triangles.stride       = 3*sizeof(int);
			meshDesc.triangles.data         = indes;

			PxToolkit::MemoryOutputStream writeBuffer;
			/*bool status = pxCooking->cookTriangleMesh(meshDesc, writeBuffer);
			if(!status)
				return;

			PxToolkit::MemoryInputData readBuffer(writeBuffer.getData(), writeBuffer.getSize());
			
			PxTriangleMeshGeometry triGeom;
			triGeom.triangleMesh = pxPhysics->createTriangleMesh(readBuffer);
			triGeom.scale = PxMeshScale(PxVec3(scale,scale,scale),PxQuat());

			meshShape = meshActor->createShape(triGeom, *pxMaterial);
			meshShape->setLocalPose(PxTransform(PxVec3(x, y, z)));

			pxScene->addActor(*meshActor);*/
	}
}

void PhysX::CreateSphere(float x, float y, float z)
{
	return;
}


//PxShape* aSphereShape;
//PxRigidDynamic *boxActor;
PxRigidActor *boxes[MAX_BOXES];
int numbox = 0;
void PhysX::CreateBox(float x, float y, float z, float lookx, float looky, float lookz, float firespeed)
{
	if(numbox >= MAX_BOXES)
		return;

	if(mCooldown > 0.0f)
		return;
	
	PxReal density = 5.0f;
	PxVec3 look = PxVec3(lookx,looky,lookz);
	look.normalize();
	PxTransform transform(PxVec3(x, y, z) + (look * 4.), PxQuat::createIdentity());
	PxVec3 dimensions(.5,.5,.5);
	PxBoxGeometry geometry(dimensions);
	PxRigidDynamic* boxActor = PxCreateDynamic(*pxPhysics, transform, geometry, *pxMaterial, density);
	if (!boxActor)
		return;

	float vx = look.x * firespeed;
	float vy = look.y * firespeed;
	float vz = look.z * firespeed;
	boxActor->setLinearVelocity(PxVec3(vx,vy,vz));
	boxActor->setAngularDamping(0.75);
	boxActor->setLinearDamping(0.8);
	PxRigidBodyExt::updateMassAndInertia(*boxActor, density);
	pxScene->addActor(*boxActor);
	boxes[numbox] = boxActor;

	mCooldown = 0.1f;
	numbox++;
}

int PhysX::GetNumBoxes()
{
    return numbox;
} 

PxTransform PhysX::GetBoxWorld(int boxnum)
{
	PxU32 nShapes = 0;
	if(boxes[boxnum])
		nShapes = boxes[boxnum]->getNbShapes();
	else	
		return PxTransform(PxVec3(0, -100., 0));

	if(numbox-1 < boxnum)
		return PxTransform(PxVec3(0, -100., 0));

	PxShape** shapes = new PxShape*[nShapes];
 
	boxes[boxnum]->getShapes(shapes, nShapes);     
	PxTransform pt = PxShapeExt::getGlobalPose(*shapes[0]);

	delete [] shapes;
	
	return pt;
}

PxTransform PhysX::GetSphereWorld()
{
	//PxU32 nShapes = 0;
	//if(boxes[boxnum])
	//	nShapes = boxes[boxnum]->getNbShapes();
	//else	
	//	return PxTransform(PxVec3(0, 0, 0),PxQuat(0,0,0,1));


	//if(nShapes > 0)
	//{
	//	PxShape** shapes = new PxShape*[nShapes];
 //
	//	box->getShapes(shapes, nShapes);     
	//	
	//	PxTransform pt = PxShapeExt::getGlobalPose(shapes[0][0]);

	//	delete [] shapes;
	//
	//	return pt;
	//}
	
}


void PhysX::Draw(PxTransform &transform)
{
	//transform = PxShapeExt::getGlobalPose(*aSphereShape);
}

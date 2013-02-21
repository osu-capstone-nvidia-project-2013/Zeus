//***************************************************************************************
// PhysXHeightField.h
//
//
// 
//
//***************************************************************************************
#include "PhysXHeightField.h"

PhysXHeightfield::PhysXHeightfield()
{

}

PhysXHeightfield::~PhysXHeightfield()
{

}

struct Heightfield mHeightfield;

void PhysXHeightfield::InitHeightfield(PxPhysics* physics, PxScene* scene, const char* filename)
{
	float xScale = 10.0;
	float yScale = 100.0;
	float zScale = 10.0;

	
	// NOTE: Assuming that heightfield texture has B8G8R8A8 format.
	if(LoadHeightfield(filename)) 
	{
		PxU16 nbColumns = PxU16(mHeightfield.width);
		PxU16 nbRows = PxU16(mHeightfield.height);

		PxHeightFieldDesc heightFieldDesc;
		heightFieldDesc.nbColumns = nbColumns;
		heightFieldDesc.nbRows = nbRows;
		heightFieldDesc.samples.data = mHeightfield.data;
		heightFieldDesc.samples.stride = sizeof(PxHeightFieldSample);
		heightFieldDesc.convexEdgeThreshold = 0;

		PxHeightField* heightField = physics->createHeightField(heightFieldDesc);
		// create shape for heightfield		
		PxTransform pose(PxVec3(-((PxReal)nbRows*yScale) / 2.0f, 
								0.0f, 
								-((PxReal)nbColumns*xScale) / 2.0f),  
						PxQuat::createIdentity());
		PxRigidActor* hf = physics->createRigidStatic(pose);
		const PxMaterial* mMat = physics->createMaterial(0.9f, 0.9f, 0.001f);
		PxShape* shape = hf->createShape((PxHeightFieldGeometry(heightField, PxMeshGeometryFlags(), yScale, xScale, zScale)), *mMat);

		//PxHeightFieldGeometry hfGeom(heightField, PxMeshGeometryFlags(), heightScale, rowScale, colScale);
		//PxShape* aHeightFieldShape = aHeightFieldActor->createShape(hfGeom, aMaterialArray, nbMaterials);

		shape->setFlag(PxShapeFlag::ePARTICLE_DRAIN, false);
		shape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, true);
		shape->setFlag(PxShapeFlag::eUSE_SWEPT_BOUNDS, true);
		// add actor to the scene
		scene->addActor(*hf);
	}
}

bool PhysXHeightfield::LoadHeightfield(const char* filename)
{
	mHeightfield.height = 2049;
	mHeightfield.width = 2049;
	// A height for each vertex
	std::vector<unsigned char> in( mHeightfield.width * mHeightfield.height );

	// Open the file.
	std::ifstream inFile;
	inFile.open(filename, std::ios_base::binary);

	if(inFile)
	{
		// Read the RAW bytes.
		inFile.read((char*)&in[0], (std::streamsize)in.size());

		// Done with file.
		inFile.close();
	}
	else
	{
		return false;
	}
	mHeightfield.data = new PxHeightFieldSample[(mHeightfield.height*mHeightfield.width)];
	// Copy the array data into a float array and scale it.
	for(int i = 0; i < mHeightfield.width * mHeightfield.height; ++i)
	{
		PxHeightFieldSample* currentSample;
		currentSample->height = (in[i] / 255.0f);
		currentSample->materialIndex0 = 0;
		currentSample->materialIndex1 = 0;
		currentSample->clearTessFlag();
		mHeightfield.data[i] = *currentSample;
	}
	return true;
}
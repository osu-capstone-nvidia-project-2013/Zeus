//***************************************************************************************
// PhysXParticles.cpp
//
//
// 
//
//***************************************************************************************
#include "PhysXParticles.h"

PxParticleSystem* ps;
PxParticleFluid* fluidps;

PhysXParticles::PhysXParticles()
{

}

PhysXParticles::~PhysXParticles()
{

}

void PhysXParticles::InitParticles(int maxParticles, PxPhysics* physics, PxScene* scene)
{
	bool perParticleRestOffset = false;
	ps = NULL;
	ps = physics->createParticleSystem(maxParticles, perParticleRestOffset);

	if (ps)
	{
		ps->setParticleBaseFlag(PxParticleBaseFlag::eCOLLISION_TWOWAY, false);
		ps->setParticleBaseFlag(PxParticleBaseFlag::eENABLED, true);
		scene->addActor(*ps);
	}
}

void PhysXParticles::CreateParticles(int count, float x, float y, float z)
{
	
	std::vector<PxVec3> positions;
	positions.push_back(PxVec3(x,y,z));

	const PxVec3* positionBuffer = positions.data();

	std::vector<PxU32> indices;
	indices.push_back(0);
	const PxU32* indicesBuffer = indices.data();

	// declare particle descriptor for creating new particles
	// based on numNewAppParticles count and newAppParticleIndices, newAppParticlePositions arrays.
	PxParticleCreationData particleCreationData;
	particleCreationData.numParticles = count;
	particleCreationData.indexBuffer = PxStrideIterator<const PxU32>(indicesBuffer, sizeof(PxU32));
	particleCreationData.positionBuffer = PxStrideIterator<const PxVec3>(positionBuffer, sizeof(PxVec3));
	
	// create particles in *PxParticleSystem* ps
	ps->createParticles(particleCreationData);
}

void PhysXParticles::ReleaseParticles()
{
	ps->releaseParticles();
}

void PhysXParticles::UpdateParticles()
{
	
}

vector<PxVec3> PhysXParticles::ReadParticlesPositions()
{
	// lock SDK buffers of *PxParticleSystem* ps for reading
	PxParticleReadData* rd = ps->lockParticleReadData();
	vector<PxVec3> positions;
	// access particle data from PxParticleReadData
	if (rd)
	{
		PxStrideIterator<const PxParticleFlags> flagsIt(rd->flagsBuffer);
		PxStrideIterator<const PxVec3> positionIt(rd->positionBuffer);

		for (unsigned i = 0; i < rd->validParticleRange; ++i, ++flagsIt, ++positionIt)
		{
				if (*flagsIt & PxParticleFlag::eVALID)
				{
					// access particle position
					const PxVec3& position = *positionIt;
					positions.push_back(position);
				}
		}

		// return ownership of the buffers back to the SDK
		rd->unlock();
	}
	return positions;
}

void PhysXParticles::InitFluidParticles(int maxParticles, PxPhysics* physics, PxScene* scene)
{
	bool perParticleRestOffset = false;

	fluidps = physics->createParticleFluid(maxParticles, perParticleRestOffset);

	if (fluidps)
		scene->addActor(*fluidps);
}

void PhysXParticles::CreateFluidParticles()
{

}

void PhysXParticles::ReleaseFluidParticles()
{

}

void PhysXParticles::UpdateFluidParticles()
{

}

void PhysXParticles::ReadFluidParticles()
{

}
//***************************************************************************************
// PhysXParticles.cpp
//
//
// 
//
//***************************************************************************************
#include "PhysXParticles.h"

PhysXParticles::PhysXParticles()
{

}

PhysXParticles::~PhysXParticles()
{

}

void PhysXParticles::InitParticles(int maxParticles, PxPhysics* physics, PxScene* scene)
{
	bool perParticleRestOffset = false;

	ps = physics->createParticleSystem(maxParticles, perParticleRestOffset);

	if (ps)
	{
		ps->setParticleBaseFlag(PxParticleBaseFlag::eCOLLISION_TWOWAY, false);
		ps->setParticleBaseFlag(PxParticleBaseFlag::eENABLED, true);
		scene->addActor(*ps);
	}
}

void PhysXParticles::CreateParticles(int count, std::vector<PxU32> indices, std::vector<PxVec3> positions)
{
	const PxVec3* positionBuffer = positions.data();

	const PxU32* indicesBuffer = indices.data();

	// declare particle descriptor for creating new particles
	// based on numNewAppParticles count and newAppParticleIndices, newAppParticlePositions arrays.
	PxParticleCreationData particleCreationData;
	particleCreationData.numParticles = count;
	particleCreationData.indexBuffer = PxStrideIterator<const PxU32>(indicesBuffer, sizeof(PxU32));
	particleCreationData.positionBuffer = PxStrideIterator<const PxVec3>(positionBuffer, sizeof(PxVec3));

	// create particles in *PxParticleSystem* ps
	bool success = ps->createParticles(particleCreationData);

}

void PhysXParticles::ReleaseParticles()
{

}

void PhysXParticles::UpdateParticles()
{

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
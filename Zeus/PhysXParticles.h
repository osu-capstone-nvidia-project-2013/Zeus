//***************************************************************************************
// PhysXParticles.h
//
//
// 
//
//***************************************************************************************
#ifndef PHYSX_PARTICLES_H
#define PHYSX_PARTICLES_H
#include "PhysX.h"
#include <vector>

using namespace std;
using namespace physx;

class PhysXParticles
{
public:
	PhysXParticles();
	~PhysXParticles();

	void InitParticles(int maxParticles, PxPhysics* physics, PxScene* scene);
	void CreateParticles(int count, vector<PxU32> indices, vector<PxVec3> positions);
	void ReleaseParticles();
	void UpdateParticles();
	
	void InitFluidParticles(int maxParticles, PxPhysics* physics, PxScene* scene);
	void CreateFluidParticles();
	void ReleaseFluidParticles();
	void UpdateFluidParticles();

private:
	PxParticleSystem* ps;
	PxParticleFluid* fluidps;
};

#endif
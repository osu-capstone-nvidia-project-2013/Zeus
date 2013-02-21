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

struct Emitter
{

};

class PhysXParticles
{
public:
	PhysXParticles();
	~PhysXParticles();

	void InitParticles(int maxParticles, PxPhysics* physics, PxScene* scene);
	void CreateParticles(int count, float x, float y, float z);
	void ReleaseParticles();
	void UpdateParticles();
	vector<PxVec3> ReadParticlesPositions();
	
	void InitFluidParticles(int maxParticles, PxPhysics* physics, PxScene* scene);
	void CreateFluidParticles();
	void ReleaseFluidParticles();
	void UpdateFluidParticles();
	void ReadFluidParticles();

private:
	
};

#endif
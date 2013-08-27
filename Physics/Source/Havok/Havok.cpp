//====================================================================================
//Havok.cpp : Havok physics implementation
//
//This code is part of Anubis Engine.
//
//Anubis Engine is a free game engine created as a fan project to be
//awesome platform for developing games!
//
//All sources can be found here:
//	https://github.com/Dgek/Anubis-Engine
//
//Demos based on Anubis Engine can be found here:
//	https://github.com/Dgek/Demos
//
//Copyright (c) 2013, Muralev Evgeny
//All rights reserved.
//
//Redistribution and use in source and binary forms, with
//or without modification, are permitted provided that the
//following conditions are met:
//
//Redistributions of source code must retain the above copyright notice,
//this list of conditions and the following disclaimer.
//
//Redistributions in binary form must reproduce the above copyright notice,
//this list of conditions and the following disclaimer in the documentation
//and/or other materials provided with the distribution.
//
//Neither the name of the Minotower Games nor the names of its contributors 
//may be used to endorse or promote products derived from this software 
//without specific prior written permission.
//
//THIS SOFTWARE IS PROVIDED BY MURALEV EVGENY "AS IS"
//AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, 
//THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
//ARE DISCLAIMED. IN NO EVENT SHALL MURALEV EVGENY BE LIABLE
//FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
//(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
//LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
//ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
//(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
//SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
//====================================================================================

#include "Physics_pch.h"
#include "Havok.h"

#include "Entities\Entity.h"
#include "Messenger.h"
#include "Message.h"
#include "GameMessages.h"

#include "Application\Engine.h"

#undef HK_FEATURE_PRODUCT_AI
#undef HK_FEATURE_PRODUCT_ANIMATION
#undef HK_FEATURE_PRODUCT_CLOTH
#undef HK_FEATURE_PRODUCT_DESTRUCTION
#undef HK_FEATURE_PRODUCT_BEHAVIOR
#undef HK_FEATURE_PRODUCT_NEW_PHYSICS
#define HK_FEATURE_REFLECTION_PHYSICS
#define HK_CLASSES_FILE <Common/Serialize/Classlist/hkClasses.h>
#define HK_EXCLUDE_FEATURE_MemoryTracker
#define HK_EXCLUDE_FEATURE_SerializeDeprecatedPre700
#define HK_EXCLUDE_FEATURE_RegisterVersionPatches 
#define HK_EXCLUDE_LIBRARY_hkGeometryUtilities

#include <Common/Base/hkBase.h>
#include <Common/Base/System/hkBaseSystem.h>
#include <Common/Base/System/Error/hkDefaultError.h>
#include <Common/Base/Memory/System/Util/hkMemoryInitUtil.h>
#include <Common/Base/Monitor/hkMonitorStream.h>
#include <Common/Base/Memory/System/hkMemorySystem.h>
#include <Common/Base/Memory/Allocator/Malloc/hkMallocAllocator.h>

#include <Physics/Dynamics/World/hkpWorld.h>
#include <Physics/Dynamics/Entity/hkpRigidBody.h>
//#include <Physics/Dynamics/hkpDynamics.h>

#include <Physics/Collide/Dispatch/hkpAgentRegisterUtil.h>
#include <Physics/Collide/Shape/Convex/Box/hkpBoxShape.h>
#include <Physics/Collide/Shape/Convex/Sphere/hkpSphereShape.h>
#include <Physics/Collide/Shape/HeightField/Plane/hkpPlaneShape.h>
//#include <Physics/Collide/Shape/Convex/Cylinder/hkpCylinderShape.h>

#include <Physics/Utilities/Dynamics/Inertia/hkpInertiaTensorComputer.h>

using namespace Anubis;

#undef HK_FEATURE_PRODUCT_AI
#undef HK_FEATURE_PRODUCT_ANIMATION
#undef HK_FEATURE_PRODUCT_CLOTH
#undef HK_FEATURE_PRODUCT_DESTRUCTION
#undef HK_FEATURE_PRODUCT_BEHAVIOR
#undef HK_FEATURE_PRODUCT_NEW_PHYSICS
#define HK_FEATURE_REFLECTION_PHYSICS
#define HK_CLASSES_FILE <Common/Serialize/Classlist/hkClasses.h>
#define HK_EXCLUDE_FEATURE_MemoryTracker
#define HK_EXCLUDE_FEATURE_SerializeDeprecatedPre700
#define HK_EXCLUDE_FEATURE_RegisterVersionPatches 
#define HK_EXCLUDE_LIBRARY_hkGeometryUtilities
#include <Common/Base/KeyCode.cxx>
#include <Common/Base/Config/hkProductFeatures.cxx>
// Platform specific initialization
#include <Common/Base/System/Init/PlatformInit.cxx>

namespace Anubis
{
	class HavokActor
	{
	protected:
		//typedef std::vector<hkpRigidBody*> RigidBodies;
		//RigidBodies
		hkpRigidBody*	m_pRigidBody;
	};

	class HavokPhysics : public IPhysics
	{
	protected:

		//////////////////////////////////////////////////
		//Havok stuff
		hkpWorld*				m_pWorld;
		hkpBroadPhase*			m_pBroadPhase;
		hkpCollisionDispatcher*	m_pCollisionDispatcher;
		hkpConstraintInstance*	m_pConstraintDispatcher;

		//////////////////////////////////////////////////
		//Havok memory
		hkMemoryRouter*			m_pMemoryRouter;

		//////////////////////////////////////////////////
		//Physical properties
		typedef std::map<ASTRING, float> Densities;
		typedef std::map<ASTRING, PhysicsMaterial> Materials;

		Densities m_densityTable;
		Materials m_materialTable;

		///////////////////////////////////////////////////
		//Actors
		typedef std::map<const EntityId, hkpRigidBody*> EntityIdToRigidBody;
		EntityIdToRigidBody m_entityIdToRigidBody;
		hkpRigidBody* GetHavokRigidBody(const EntityId entityId) const;

		typedef std::map<const hkpRigidBody*, EntityId> RigidBodyToEntityId;
		RigidBodyToEntityId m_rigidBodyToEntityId;
		EntityId GetEntityId(const hkpRigidBody* pRigidBody) const;

	public:

		HavokPhysics();
		AVIRTUAL ~HavokPhysics();

		////////////////////////////////////
		//Load Data
		AVOID LoadDataFromXml();

		/**********************************
		//	Implement IPhysics Interface
		**********************************/

		//Initialize physics
		AVIRTUAL ABOOL VInit();
		AVOID LoadData();
		
		//sychronize with engine
		AVIRTUAL AVOID VSyncData(AREAL64 r64CurrentTime);

		//Update simulation
		AVIRTUAL AVOID VUpdate(AREAL deltaSeconds );

		/////////////////////////////////////
		//add objects
		//AVIRTUAL AVOID VAddShape(EntityPtr pEntity, hkpShape* pShape, AREAL r32Mass, const ASTRING & physicsMaterial, 
		//	const ABOOL isStatic = false);
		AVIRTUAL AVOID VAddShape(EntityPtr pEntity, hkpShape * pShape, hkpRigidBodyCinfo & ci);
		AVIRTUAL AVOID VAddBox(EntityPtr pEntity, Vec & pos, Vec & dimensions, AREAL r32Mass, ASTRING & physMat, ABOOL isStatic = false);
		AVIRTUAL AVOID VAddPlane(EntityPtr pEntity, Vec & pos, Vec & dimensions, AREAL r32Mass, ASTRING & physMat, ABOOL isStatic = false);
		//AVIRTUAL AVOID VAddSphere(EntityPtr pEntity, const AREAL radius, 

		/////////////////////////////////////
		//physics modifiers
		AVIRTUAL AVOID VApplyForce(const EntityId entityId, const AREAL r32DeltaTime, const Vec & dir, const AREAL newtons);
		AVIRTUAL AVOID VApplyTorque(const EntityId entityId, const AREAL r32DeltaTime, const Vec & dir, const AREAL newtons);
		AVIRTUAL AVOID VKinematicMovement(const EntityId entityId, const Mat4x4 & mat);

		/////////////////////////////////////
		//getters
		AVIRTUAL Vec VGetVelocity(const EntityId entityId);
		AVIRTUAL Vec VGetAngularVelocity(const EntityId entityId);
		AVIRTUAL Mat4x4 VGetTransform(const EntityId entityId);

		/////////////////////////////////////
		//setters
		AVIRTUAL AVOID VSetVelocity(const EntityId entityId, const Vec & vel);
		AVIRTUAL AVOID VSetAngularVelocity(const EntityId entityId, const Vec & angularVel);
		//AVIRTUAL AVOID VSetTransform(const EntityId entityId, const Mat4x4 & mat);
	};

};
/////////////////////////////////////////
//Conversion helpers
/////////////////////////////////////////
static Vec hkVector4_to_Vec( const hkVector4 & vec )
{
	return Vector(vec.getComponent(0), vec.getComponent(1), vec.getComponent(2), vec.getW());
}

static hkVector4 Vec_to_hkVector4( const Vec & vec )
{
	return hkVector4(vec.x, vec.y, vec.z, vec.w);
}

static Anubis::Mat4x4 hkTransform_to_Mat4x4( const hkTransform & mat )
{
	/*return Mat4x4(	mat.getElement<0, 0>(), mat.getElement<0, 1>(), mat.getElement<0, 2>(), mat.getElement<0, 3>(),
					mat.getElement<1, 0>(), mat.getElement<1, 1>(), mat.getElement<1, 2>(), mat.getElement<1, 3>(),
					mat.getElement<2, 0>(), mat.getElement<2, 1>(), mat.getElement<2, 2>(), mat.getElement<2, 3>(),
					mat.getElement<3, 0>(), mat.getElement<3, 1>(), mat.getElement<3, 2>(), mat.getElement<3, 3>() ); */
	//hkTransform is column-major, so transpose
	return Mat4x4(	mat.getElement<0, 0>(), mat.getElement<1, 0>(), mat.getElement<2, 0>(), mat.getElement<3, 0>(),
					mat.getElement<0, 1>(), mat.getElement<1, 1>(), mat.getElement<2, 1>(), mat.getElement<3, 1>(),
					mat.getElement<0, 2>(), mat.getElement<1, 2>(), mat.getElement<2, 2>(), mat.getElement<3, 2>(),
					mat.getElement<0, 3>(), mat.getElement<1, 3>(), mat.getElement<2, 3>(), mat.getElement<3, 3>() );
};


static hkTransform Mat4x4_to_hkTransform( const Mat4x4 & mat )
{
	//hkTransform is column-major, so transpose
	hkTransform transform;
	transform.setElement<0, 0>(mat.GetElement(0, 0));
	transform.setElement<0, 1>(mat.GetElement(1, 0));
	transform.setElement<0, 2>(mat.GetElement(2, 0));
	transform.setElement<0, 3>(mat.GetElement(3, 0));

	transform.setElement<1, 0>(mat.GetElement(0, 1));
	transform.setElement<1, 1>(mat.GetElement(1, 1));
	transform.setElement<1, 2>(mat.GetElement(2, 1));
	transform.setElement<1, 3>(mat.GetElement(3, 1));

	transform.setElement<2, 0>(mat.GetElement(0, 2));
	transform.setElement<2, 1>(mat.GetElement(1, 2));
	transform.setElement<2, 2>(mat.GetElement(2, 2));
	transform.setElement<2, 3>(mat.GetElement(3, 2));

	transform.setElement<3, 0>(mat.GetElement(0, 3));
	transform.setElement<3, 1>(mat.GetElement(1, 3));
	transform.setElement<3, 2>(mat.GetElement(2, 3));
	transform.setElement<3, 3>(mat.GetElement(3, 3));

	return transform;
}

/////////////////////////////////////////
//Havok Physics
/////////////////////////////////////////

ASTATIC AVOID HK_CALL errorReport(const ACHAR* msg, APVOID userArgGivenToInit)
{
	OutputDebugStringA(msg);
	//using namespace std;
	//printf("%s", msg);
}

HavokPhysics::HavokPhysics()
{
	m_pWorld = nullptr;
	//TODO:register events here
}

HavokPhysics::~HavokPhysics()
{
	SAFE_DELETE(m_pWorld);
	SAFE_DELETE(m_pBroadPhase);
	SAFE_DELETE(m_pCollisionDispatcher);
	SAFE_DELETE(m_pConstraintDispatcher);
}

AVOID HavokPhysics::LoadData()
{
	//////////////////////////////////////////////
	//Load Material data
	//////////////////////////////////////////////
	TiXmlDocument materialDoc("Physics\\Properties\\Materials.xml");
	materialDoc.LoadFile();

	TiXmlElement* pRoot = materialDoc.FirstChildElement("PhysicsMaterials");
	assert(pRoot && "Cannot load density table from Physics\\Properties\\Materials.xml!!!");
	if (pRoot)
	{
		for (TiXmlElement* pElem = pRoot->FirstChildElement(); pElem; pElem = pElem->NextSiblingElement())
		{
			AREAL64 friction = 0;
			AREAL64 restitution = 0;

			pElem->Attribute("friction", &friction);
			pElem->Attribute("restitution", &restitution);

			m_materialTable.insert(make_pair(pElem->Value(), PhysicsMaterial(restitution, friction)));
		}
	}

	/////////////////////////////////////////////////
	//Load Densities
	/////////////////////////////////////////////////
	TiXmlDocument densityDoc("Physics\\Properties\\DensityTable.xml");
	densityDoc.LoadFile();

	pRoot = densityDoc.FirstChildElement("DensityTable");
	assert(pRoot && "Cannot load density table from Physics\\Properties\\DensityTable.xml!!!");
	if (pRoot)
	{
		for (TiXmlElement* pElem = pRoot->FirstChildElement(); pElem; pElem = pElem->NextSiblingElement())
		{
			AREAL64 density = 0;

			m_densityTable.insert(make_pair(pElem->Value(), atof(pElem->FirstChild()->Value())));
		}
	}
}

ABOOL HavokPhysics::VInit() 
{ 
	LoadData();

	//Initialize memory
	m_pMemoryRouter = hkMemoryInitUtil::initDefault( hkMallocAllocator::m_defaultMallocAllocator, hkMemorySystem::FrameInfo( 1024 * 1024 ) );
	hkBaseSystem::init( m_pMemoryRouter, errorReport );

	//hkMonitorStream::getInstance().resize(200000);

	//////////////////////////////////////
	//Create hkpWorld
	hkpWorldCinfo info;
	info.m_gravity.set(0, -9.8f, 0);
	info.setBroadPhaseWorldSize(3000.0f);
	info.m_broadPhaseBorderBehaviour = hkpWorldCinfo::BROADPHASE_BORDER_DO_NOTHING;
	info.setupSolverInfo(hkpWorldCinfo::SOLVER_TYPE_4ITERS_MEDIUM);
	info.m_simulationType = hkpWorldCinfo::SIMULATION_TYPE_CONTINUOUS; 

	m_pWorld = new hkpWorld(info);

	//register all xollision agents
	hkpAgentRegisterUtil::registerAllAgents(m_pWorld->getCollisionDispatcher());

	return true; 
}
		
//Update simulation
AVOID HavokPhysics::VUpdate(AREAL deltaSeconds ) 
{
	m_pWorld->stepDeltaTime(deltaSeconds);                                               
}

AVOID HavokPhysics::VSyncData(AREAL64 r64CurrentTime)
{
	//Synchronize graphics with physics
	for (auto it = m_entityIdToRigidBody.begin(); it != m_entityIdToRigidBody.end(); it++)
	{
		const EntityId id = it->first;
		EntityPtr pEntity = g_pEngine->m_pGame->VGetEntity(id);

		hkpRigidBody* const pRigidBody = it->second;
		//const hkMotionState * const pMotionState = pRigidBody->getMotionState();

		Mat4x4 transform = hkTransform_to_Mat4x4(pRigidBody->getTransform());
		transform.rows[3].w = 1.0f;
		if (pEntity->GetGurrentTransform() != transform)
		{
			pEntity->SetCurrentTransform(transform, r64CurrentTime);
			//Anubis::SendGameMessage(Mes_Move_Entity(id, transform));
		}
	} 
}

AVOID HavokPhysics::VAddShape(EntityPtr pEntity, hkpShape* pShape,  hkpRigidBodyCinfo & ci)
{
	//Create rigid body
	hkpRigidBody* pRigidBody = new hkpRigidBody(ci);

	//Remove reference to the shape as rigid body holds it
	//pShape->removeReference();

	////////////////////////////////////////////
	//add rigid body to help game structures
	m_entityIdToRigidBody.insert(std::make_pair(pEntity->GetId(), pRigidBody));
	m_rigidBodyToEntityId.insert(std::make_pair(pRigidBody, pEntity->GetId()));

	////////////////////////////////////////////
	//Add body to the physics world and remove reference
	m_pWorld->lock();
	m_pWorld->addEntity(pRigidBody);//->removeReference();

	pRigidBody->removeReference();
	pShape->removeReference();

	m_pWorld->unlock();
}

AVOID HavokPhysics::VAddBox(EntityPtr pEntity, Vec & pos, Vec & halfDimensions, AREAL r32Mass, ASTRING & physMat, ABOOL isStatic)
{
	hkVector4 halfExtents = Vec_to_hkVector4(halfDimensions);
	hkpBoxShape* pBoxShape = new hkpBoxShape(halfExtents);

	////////////////////////////////////
	//Set properties
	hkpRigidBodyCinfo ci;
	ci.m_shape = pBoxShape;
	ci.m_position = Vec_to_hkVector4(pos);
	if (isStatic)
		ci.m_motionType = hkpMotion::MOTION_FIXED;
	else
	{
		//set motion type
		ci.m_motionType = hkpMotion::MOTION_DYNAMIC;

		//set mass properties
		hkMassProperties massProperties;
		hkpInertiaTensorComputer::computeShapeVolumeMassProperties(pBoxShape, r32Mass, massProperties);

		//ci.setMassProperties(massProperties);
		//ci.m_centerOfMass = hkVector4(0.0f, 0.0f, 0.0f, 1.0f);
		//ci.m_mass = r32Mass;
	}

	//set material properties
	PhysicsMaterial material = m_materialTable[physMat];
	ci.m_friction = material.m_r32Friction;
	ci.m_restitution = material.m_r32Restitution;

	VAddShape(pEntity, pBoxShape, ci);
}

AVOID HavokPhysics::VAddPlane(EntityPtr pEntity, Vec & pos, Vec & halfDimensions, AREAL r32Mass, ASTRING & physMat, ABOOL isStatic)
{
	//hkVector4 halfExtents = Vec_to_hkVector4(halfDimensions);
	//hkpPlaneShape* pPlaneShape = new hkpPlaneShape(hkVector4(0.0f, 0.0f, 1.0f, 0.0f), Vec_to_hkVector4(pos), halfExtents);
	hkVector4 halfExtents = hkVector4(halfDimensions.x, 2.0f, halfDimensions.y, 0.0f);
	hkpBoxShape* pPlaneShape = new hkpBoxShape(halfExtents);

	////////////////////////////////////
	//Set properties
	hkpRigidBodyCinfo ci;
	ci.m_shape = pPlaneShape;
	ci.m_position = Vec_to_hkVector4(pos);
	if (isStatic)
		ci.m_motionType = hkpMotion::MOTION_FIXED;
	else
	{
		//set motion type
		ci.m_motionType = hkpMotion::MOTION_DYNAMIC;

		//set mass properties
		hkMassProperties massProperties;
		hkpInertiaTensorComputer::computeShapeVolumeMassProperties(pPlaneShape, r32Mass, massProperties);

		ci.setMassProperties(massProperties);
	}

	//set material properties
	PhysicsMaterial material = m_materialTable[physMat];
	ci.m_friction = material.m_r32Friction;
	ci.m_restitution = material.m_r32Restitution;

	VAddShape(pEntity, pPlaneShape, ci);
}


/////////////////////////////////////////////////
//Appliers
/////////////////////////////////////////////////
AVOID HavokPhysics::VApplyForce(const EntityId entityId, const AREAL r32DeltaTime, const Vec & dir, const AREAL newtons)
{
	hkpRigidBody* pRigidBody = m_entityIdToRigidBody[entityId];
	pRigidBody->applyForce(r32DeltaTime, Vec_to_hkVector4(dir * newtons));
}

AVOID HavokPhysics::VApplyTorque(const EntityId entityId, const AREAL r32DeltaTime, const Vec & dir, const AREAL newtons)
{
	hkpRigidBody* pRigidBody = m_entityIdToRigidBody[entityId];
	pRigidBody->applyTorque(r32DeltaTime, Vec_to_hkVector4(dir * newtons));
}

AVOID HavokPhysics::VKinematicMovement(const EntityId entityId, const Mat4x4 & mat)
{
	hkpRigidBody* pRigidBody = m_entityIdToRigidBody[entityId];
	pRigidBody->setTransform(Mat4x4_to_hkTransform(mat));
}

/////////////////////////////////////////////////
//Getters
/////////////////////////////////////////////////
Vec HavokPhysics::VGetVelocity(const EntityId entityId)
{
	hkpRigidBody* pRigidBody = m_entityIdToRigidBody[entityId];
	return hkVector4_to_Vec(pRigidBody->getLinearVelocity());
}

Vec HavokPhysics::VGetAngularVelocity(const EntityId entityId)
{
	hkpRigidBody* pRigidBody = m_entityIdToRigidBody[entityId];
	return hkVector4_to_Vec(pRigidBody->getAngularVelocity());
}

Mat4x4 HavokPhysics::VGetTransform(const EntityId entityId)
{
	hkpRigidBody* pRigidBody = m_entityIdToRigidBody[entityId];
	return hkTransform_to_Mat4x4(pRigidBody->getTransform());
}

/////////////////////////////////////////////////
//Setters
/////////////////////////////////////////////////
AVOID HavokPhysics::VSetVelocity(const EntityId entityId, const Vec & vel)
{
	hkpRigidBody* pRigidBody = m_entityIdToRigidBody[entityId];
	pRigidBody->setLinearVelocity(Vec_to_hkVector4(vel));
}

AVOID HavokPhysics::VSetAngularVelocity(const EntityId entityId, const Vec & angularVel)
{
	hkpRigidBody* pRigidBody = m_entityIdToRigidBody[entityId];
	pRigidBody->setAngularVelocity(Vec_to_hkVector4(angularVel));
}

/////////////////////////////////////////////////////
//Return global physics instance
/////////////////////////////////////////////////////
IPhysics* Anubis::CreateHavokPhysics()
{
	return new HavokPhysics();
}
#include "VSMDemo_pch.h"
#include "ShadowTestState.h"

#include "Application\Engine.h"

#include "../Entities/BoxEntity/BoxEntityResource.h"
#include "../Entities/BoxEntity/BoxEntity.h"
#include "../Entities/BoxEntity/BoxEntityRepresentation.h"

#include "../Entities/ObstacleEntity/ObstacleEntityResource.h"
#include "../Entities/ObstacleEntity/ObstacleEntity.h"
#include "../Entities/ObstacleEntity/ObstacleEntityRepresentation.h"

#include "../Entities/PlaneEntity/PlaneEntity.h"

#include "../Entities/SpotLightEntity/SpotLightEntityResource.h"
#include "../Entities/SpotLightEntity/SpotLightEntity.h"
#include "../Entities/SpotLightEntity/SpotLightEntityRepresentation.h"


using namespace Anubis;

//Initialize game state
AVOID ShadowTestState::VInitialize( Game * pGame, AUINTPTR uptrData)
{
	m_pScene->VInitialize();

	// **** Generate entities **** //

	//create ground
	PlaneEntityResource & plane_params = PlaneEntityResource();
	//plane_params.m_pos = Vector(0.0f, -4.0f, 0.0f, 1.0f);
	plane_params.VLoad("Resources\\JSON\\wall1_params.json");
	EntityPtr pPlane =  pGame->VAddEntity(plane_params.VCreateEntity(pGame));
	plane_params.VCreateRepresentation(m_pScene, pPlane); 
	plane_params.VCreatePhysicalBody(g_pEngine->GetPhysics(), pPlane); 

	BoxEntityResource & box_params = BoxEntityResource();
	box_params.VLoad("Resources\\JSON\\box1_params.json");
	EntityPtr pBox = pGame->VAddEntity(box_params.VCreateEntity(pGame));
	box_params.VCreateRepresentation(m_pScene, pBox);
	box_params.VCreatePhysicalBody(g_pEngine->GetPhysics(), pBox); 


	//create resource
	ObstacleEntityResource & obstacle_params = ObstacleEntityResource("Hawkgirl_B.obj", Vector(0.0f, 0.0f, 0.0f, 1.0f));
	//ObstacleEntityResource & obstacle_params = ObstacleEntityResource("Chest.obj", Vector(0.0f, 0.0f, 0.0f, 1.0f));
	EntityPtr pObstacle = pGame->VAddEntity(obstacle_params.VCreateEntity(pGame));
	obstacle_params.VCreateRepresentation(m_pScene, pObstacle); 
	obstacle_params.VCreatePhysicalBody(g_pEngine->GetPhysics(), pObstacle); 

	//add point light
	//PointLightEntityResource light_params = PointLightEntityResource();
	//EntityPtr pLight = pGame->VAddEntity(light_params.VCreateEntity(pGame));
	//light_params.VCreateRepresentation(m_pScene, pLight);
	
	SpotLightEntityResource spotLight_params = SpotLightEntityResource(Vector(1.0f, 1.0f, 1.0f, 1.0f), Vector(-10.0f, 4.0f, -15.0f, 1.0f), Vector(0.4f, -0.5f, 0.8f, 0.0f),
		(AREAL)Pi / 8, (AREAL)Pi / 4, 200.0f);
	//SpotLightEntityResource spotLight_params = SpotLightEntityResource(Vector(1.0f, 1.0f, 1.0f, 1.0f), Vector(0.0f, 16.0f, 0.0f, 1.0f), Vector(0.0f, -1.0f, 0.0f, 0.0f),
	//	(AREAL)Pi / 8, (AREAL)Pi / 4, 30.0f);
	m_pLight = pGame->VAddEntity(spotLight_params.VCreateEntity(pGame));
	spotLight_params.VCreateRepresentation(m_pScene, m_pLight); 

	//spotLight_params = SpotLightEntityResource(Vector(1.0f, 1.0f, 1.0f, 1.0f), Vector(-18.0f, 4.0f, 0.0f, 1.0f), Vector(0.6f, -0.3f, 0.0f, 0.0f), 
	//	(AREAL)Pi / 16, (AREAL)Pi / 6, 50.0f);
	//EntityPtr pSecondLight = pGame->VAddEntity(spotLight_params.VCreateEntity(pGame));
	//spotLight_params.VCreateRepresentation(m_pScene, pSecondLight);

}

//Destroy the state
AVOID ShadowTestState::VRelease( Game * pGame ) 
{
	SAFE_DELETE(m_pScene);
}

//Update the state
AVOID ShadowTestState::VUpdate( Game * pGame, AREAL64 r64Time, AREAL64 r64ElapsedTime )
{
	m_pScene->VUpdate(r64Time, r64ElapsedTime);

	//update light
	AREAL diffX = r64ElapsedTime * 5.0f;
	Vec trans = Vector(40.0f*Cos(r64Time), 8, 40.0f*Sin(r64Time), 1);
	//Vec trans = Vector(-15, -2.0f, -8.0f, 1);
	Mat4x4 tr;
	tr.CreateTranslation(trans);

	SpotLightEntity* pLight = static_cast<SpotLightEntity*>(&*m_pLight);
	pLight->SetCurrentTransform(tr, r64Time);

	Vec dir = Normalize(Vector(0.0f, 0.0f, 0.0f, 1.0f) - trans);
	pLight->SetDirection(dir);
	//pLight->SetDirection( Vector(0.4f, -0.5f, 0.8f, 2.0f) );
}

//Render State
AVOID ShadowTestState::VRender( Game * pGame, Renderer * pRenderer, AREAL64 r64Time, AREAL64 r64ElapsedTime )
{
	m_pScene->VRender(pRenderer, r64Time);
}
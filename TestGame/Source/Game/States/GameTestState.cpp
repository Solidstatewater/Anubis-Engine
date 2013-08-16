#include "TestGame_pch.h"
#include "GameTestState.h"

#include "Application\Engine.h"

#include "../../Entities/BoxEntity/BoxEntityResource.h"
#include "../../Entities/BoxEntity/BoxEntity.h"
#include "../../Entities/BoxEntity/BoxEntityRepresentation.h"

#include "../../Entities/ObstacleEntity/ObstacleEntityResource.h"
#include "../../Entities/ObstacleEntity/ObstacleEntity.h"
#include "../../Entities/ObstacleEntity/ObstacleEntityRepresentation.h"

#include "../../Entities/PlaneEntity/PlaneEntity.h"

#include "../../Entities/PointLightEntity/PointLightEntityResource.h"
#include "../../Entities/PointLightEntity/PointLightEntity.h"
#include "../../Entities/PointLightEntity/PointLightEntityRepresentation.h"

#include "../../Entities/SpotLightEntity/SpotLightEntityResource.h"
#include "../../Entities/SpotLightEntity/SpotLightEntity.h"
#include "../../Entities/SpotLightEntity/SpotLightEntityRepresentation.h"

#include "../../Objects/Vegetation/GrassRepresentation.h"

using namespace Anubis;

//Initialize game state
AVOID GameTestState::VInitialize( Game * pGame, AUINTPTR uptrData)
{
	m_pScene->VInitialize();

	// **** Generate entities **** //

	//create resource
	BoxEntityResource & box_params = BoxEntityResource();
	box_params.VLoad("Resources\\JSON\\box1_params.json");
	EntityPtr pBox = pGame->VAddEntity(box_params.VCreateEntity(pGame));
	box_params.VCreateRepresentation(m_pScene, pBox);
	box_params.VCreatePhysicalBody(g_pEngine->GetPhysics(), pBox);

	//create second box 
	box_params.VLoad("Resources\\JSON\\box2_params.json");
	pBox = pGame->VAddEntity(box_params.VCreateEntity(pGame));
	box_params.VCreateRepresentation(m_pScene, pBox);
	box_params.VCreatePhysicalBody(g_pEngine->GetPhysics(), pBox);

	box_params.VLoad("Resources\\JSON\\box3_params.json");
	pBox = pGame->VAddEntity(box_params.VCreateEntity(pGame));
	box_params.VCreateRepresentation(m_pScene, pBox);
	box_params.VCreatePhysicalBody(g_pEngine->GetPhysics(), pBox);

	box_params.VLoad("Resources\\JSON\\box5_params.json");
	pBox = pGame->VAddEntity(box_params.VCreateEntity(pGame));
	box_params.VCreateRepresentation(m_pScene, pBox); 
	box_params.VCreatePhysicalBody(g_pEngine->GetPhysics(), pBox);
	
	//create back wall
	PlaneEntityResource & plane_params = PlaneEntityResource();
	plane_params.m_pos = Vector(0.0f, -15.0f, 0.0f, 1.0f);
	plane_params.VLoad("Resources\\JSON\\wall1_params.json");
	EntityPtr pPlane =  pGame->VAddEntity(plane_params.VCreateEntity(pGame));
	plane_params.VCreateRepresentation(m_pScene, pPlane); 
	plane_params.VCreatePhysicalBody(g_pEngine->GetPhysics(), pPlane); 

	////ObstacleEntityResource & obstacle_params = ObstacleEntityResource("Chest.obj", Vector(0.0f, 0.0f, 0.0f, 1.0f));
	//EntityPtr pObstacle = pGame->VAddEntity(obstacle_params.VCreateEntity(pGame));
	//obstacle_params.VCreateRepresentation(m_pScene, pObstacle); 
	//obstacle_params.VCreatePhysicalBody(g_pEngine->GetPhysics(), pObstacle); 

	//add point light
	//PointLightEntityResource light_params = PointLightEntityResource();
	//EntityPtr pLight = pGame->VAddEntity(light_params.VCreateEntity(pGame));
	//light_params.VCreateRepresentation(m_pScene, pLight);

	SpotLightEntityResource spotLight_params = SpotLightEntityResource(Vector(1.0f, 1.0f, 1.0f, 1.0f), Vector(0.0f, 0.0f, -12.0f, 1.0f), Vector(0.0f, 0.0f, 1.0f, 0.0f),
		(AREAL)Pi / 8, (AREAL)Pi / 4, 30.0f);
	EntityPtr pLight = pGame->VAddEntity(spotLight_params.VCreateEntity(pGame));
	spotLight_params.VCreateRepresentation(m_pScene, pLight);

	GrassRepresentation* pGrass = new GrassRepresentation(L"Resources\\Textures\\quick_grass.png",
		L"Resources\\Textures\\grass_mask.jpg",
		L"Resources\\Textures\\grass_specular.jpg", float3(10, 10, 2), Vector(0.0f, -5.0f, -4.0f, 1.0f), 
		4.0f, 4.0f, 100);
	m_pScene->AddStaticObject(pGrass); 
}

//Destroy the state
AVOID GameTestState::VRelease( Game * pGame ) 
{
	SAFE_DELETE(m_pScene);
}

//Update the state
AVOID GameTestState::VUpdate( Game * pGame, AREAL64 r64Time, AREAL64 r64ElapsedTime )
{
	m_pScene->VUpdate(r64Time, r64ElapsedTime);
}

//Render State
AVOID GameTestState::VRender( Game * pGame, Renderer * pRenderer, AREAL64 r64Time, AREAL64 r64ElapsedTime )
{
	m_pScene->VRender(pRenderer, r64Time);
}
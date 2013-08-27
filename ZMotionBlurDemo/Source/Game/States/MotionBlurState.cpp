#include "ZMotionBlur_pch.h"
#include "MotionBlurState.h"

#include "Application\Engine.h"

//#include "../Entities/BoxEntity/BoxEntityResource.h"
//#include "../Entities/BoxEntity/BoxEntity.h"
//#include "../Entities/BoxEntity/BoxEntityRepresentation.h"

//#include "../Entities/ObstacleEntity/ObstacleEntityResource.h"
//#include "../Entities/ObstacleEntity/ObstacleEntity.h"
//#include "../Entities/ObstacleEntity/ObstacleEntityRepresentation.h"

//#include "../Entities/PlaneEntity/PlaneEntity.h"

//#include "../Entities/SpotLightEntity/SpotLightEntityResource.h"
//#include "../Entities/SpotLightEntity/SpotLightEntity.h"
//#include "../Entities/SpotLightEntity/SpotLightEntityRepresentation.h"


using namespace Anubis;

//Initialize game state
AVOID MotionBlurState::VInitialize( Game * pGame, AUINTPTR uptrData)
{
	m_pScene->VInitialize();

	//add point light
	//PointLightEntityResource light_params = PointLightEntityResource();
	//EntityPtr pLight = pGame->VAddEntity(light_params.VCreateEntity(pGame));
	//light_params.VCreateRepresentation(m_pScene, pLight);
	
	//SpotLightEntityResource spotLight_params = SpotLightEntityResource(Vector(1.0f, 1.0f, 1.0f, 1.0f), Vector(-5.0f, 4.0f, -10.0f, 1.0f), Vector(0.4f, -0.5f, 0.8f, 0.0f),
	//	(AREAL)Pi / 8, (AREAL)Pi / 4, 30.0f);
	//SpotLightEntityResource spotLight_params = SpotLightEntityResource(Vector(1.0f, 1.0f, 1.0f, 1.0f), Vector(0.0f, 16.0f, 0.0f, 1.0f), Vector(0.0f, -1.0f, 0.0f, 0.0f),
	//	(AREAL)Pi / 8, (AREAL)Pi / 4, 30.0f);
	//EntityPtr pLight = pGame->VAddEntity(spotLight_params.VCreateEntity(pGame));
	//spotLight_params.VCreateRepresentation(m_pScene, pLight); 

	//spotLight_params = SpotLightEntityResource(Vector(1.0f, 1.0f, 1.0f, 1.0f), Vector(-18.0f, 4.0f, 0.0f, 1.0f), Vector(0.6f, -0.3f, 0.0f, 0.0f), 
	//	(AREAL)Pi / 16, (AREAL)Pi / 6, 50.0f);
	//EntityPtr pSecondLight = pGame->VAddEntity(spotLight_params.VCreateEntity(pGame));
	//spotLight_params.VCreateRepresentation(m_pScene, pSecondLight);

}

//Destroy the state
AVOID MotionBlurState::VRelease( Game * pGame ) 
{
	SAFE_DELETE(m_pScene);
}

//Update the state
AVOID MotionBlurState::VUpdate( Game * pGame, AREAL64 r64Time, AREAL64 r64ElapsedTime )
{
	m_pScene->VUpdate(r64Time, r64ElapsedTime);
}

//Render State
AVOID MotionBlurState::VRender( Game * pGame, Renderer * pRenderer, AREAL64 r64Time, AREAL64 r64ElapsedTime )
{
	m_pScene->VRender(pRenderer, r64Time);
}
#include "TestGame_pch.h"
#include "ObstacleEntityResource.h"

#include "ObstacleEntity.h"
#include "ObstacleEntityRepresentation.h"

#include "Data\JSONLoader.h"

#include "Application\Engine.h"

#include "../../Math/Source/Mat4x4.h"

#include <Allocator.h>

using namespace Anubis;

EntityResource & ObstacleEntityResource::VCopy()
{
	return static_cast<EntityResource &>(*this);
}

//create entity
EntityPtr ObstacleEntityResource::VCreateEntity(Game * pGame)
{
	return pGame->VAddEntity(make_shared<Entity>(ObstacleEntity(pGame->GetNewEntityId())));
}

EntityRepresentation* ObstacleEntityResource::VCreateRepresentation(Scene * pScene, EntityPtr pEntity)
{
	Allocator allocator;
	APVOID pMemory = allocator.allocateAligned(sizeof(ObstacleEntityRepresentation), 16);

	ObstacleEntityRepresentation * pObstaclePointer = new (pMemory) ObstacleEntityRepresentation(pEntity->GetId(), m_resourceMesh);
	pEntity->SetRepresentation(pObstaclePointer);

	Mat4x4 trans;
	trans.CreateTranslation(m_pos);
	pObstaclePointer->SetBothTransforms(trans, g_pEngine->GameTimeInSeconds());
	pScene->AddRepresentation(pObstaclePointer);

	return pObstaclePointer;
}

AVOID ObstacleEntityResource::VCreatePhysicalBody(IPhysics * pPhysics, EntityPtr pEntity)
{
	//pPhysics->VAddBox(pEntity, m_pos, Vector(m_r32X, m_r32Y, m_r32Z, 0.0f), 10.0, ASTRING("Normal"), false);
	pPhysics->VAddBox(pEntity, m_pos, Vector(5, 5, 5, 1.0f), 10.0, ASTRING("Normal"), true);
} 
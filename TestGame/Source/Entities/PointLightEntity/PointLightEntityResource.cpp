#include "TestGame_pch.h"
#include "PointLightEntityResource.h"

#include "PointLightEntity.h"
#include "PointLightEntityRepresentation.h"

#include <Allocator.h>

EntityResource & PointLightEntityResource::VCopy()
{
	return static_cast<EntityResource &>(*this);
}

//create entity
EntityPtr PointLightEntityResource::VCreateEntity(Game * pGame)
{
	return pGame->VAddEntity(make_shared<Entity>(PointLightEntity(pGame->GetNewEntityId())));
}

EntityRepresentation* PointLightEntityResource::VCreateRepresentation(Scene * pScene, EntityPtr pEntity)
{
	Allocator allocator;
	APVOID pMemory = allocator.allocateAligned(sizeof(PointLightEntityRepresentation), 16);
	PointLightEntityRepresentation * pLightPointer = new (pMemory) PointLightEntityRepresentation(pEntity->GetId(), m_color, m_pos, m_r32Range);
	pScene->AddRepresentation(pLightPointer);

	return pLightPointer;
}

AVOID PointLightEntityResource::VCreatePhysicalBody(IPhysics * pPhysics, EntityPtr pEntity)
{
}
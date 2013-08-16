#include "TestGame_pch.h"
#include "SpotLightEntityResource.h"

#include "SpotLightEntity.h"
#include "SpotLightEntityRepresentation.h"

#include <Allocator.h>

EntityResource & SpotLightEntityResource::VCopy()
{
	return static_cast<EntityResource &>(*this);
}

//create entity
EntityPtr SpotLightEntityResource::VCreateEntity(Game * pGame)
{
	return pGame->VAddEntity(make_shared<Entity>(SpotLightEntity(pGame->GetNewEntityId())));
}

EntityRepresentation* SpotLightEntityResource::VCreateRepresentation(Scene * pScene, EntityPtr pEntity)
{
	Allocator allocator;
	APVOID pMemory = allocator.allocateAligned(sizeof(SpotLightEntityRepresentation), 16);
	SpotLightEntityRepresentation * pLightPointer = new (pMemory) SpotLightEntityRepresentation(pEntity->GetId(), m_color, m_pos, m_dir, m_r32InnerRadius, m_r32OuterRadius, m_r32Range);
	pScene->AddRepresentation(pLightPointer);

	return pLightPointer;
}

AVOID SpotLightEntityResource::VCreatePhysicalBody(IPhysics * pPhysics, EntityPtr pEntity)
{
}
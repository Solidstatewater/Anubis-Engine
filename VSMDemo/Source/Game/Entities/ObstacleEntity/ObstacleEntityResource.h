#pragma once
//========================================================================
// ObstacleEntityResource.h
//
// This code is part of Minotower Games Engine 1.0v
//
// (c) Copyright 2012 Muralev Evgeny
//========================================================================

#include "Entities\EntityResource.h"
#include "../../Math/Source/Vectors.h"

struct ObstacleEntityResource : public EntityResource
{
	ASTRING m_resourceMesh;

	ObstacleEntityResource(const ASTRING & resourceMesh, Vec & pos) : EntityResource()
	{
		m_resourceMesh = resourceMesh;
		m_pos = pos;
	}

	AVOID VLoad(const ASTRING & filename) {}

	~ObstacleEntityResource() {}

	//create entity
	EntityPtr	VCreateEntity(Game * pGame);

	//create its representation
	EntityRepresentation* VCreateRepresentation(Scene * pScene, EntityPtr pEntity);

	//create its physics model
	AVOID VCreatePhysicalBody(IPhysics * pPhysics, EntityPtr pEntity);
	
	/// ***	Copy resource data *** ///
	EntityResource & VCopy();
};
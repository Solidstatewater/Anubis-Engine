#pragma once
//========================================================================
// PlaneEntityRepresentation.h
//
// This code is part of Minotower Games Engine 1.0v
//
// (c) Copyright 2012 Muralev Evgeny
//========================================================================

#include "Representations\Representation.h"
#include "Entities\EntityResource.h"

struct PlaneEntityResource : public EntityResource
{
	/***	Plane size	***/
	AREAL	m_r32Width;
	AREAL	m_r32Height;

	PlaneEntityResource() : EntityResource()
	{
	}

	AVOID VLoad(const ASTRING & filename);

	~PlaneEntityResource() {}

	//create entity
	EntityPtr	VCreateEntity(Game * pGame);

	//create its representation
	EntityRepresentation* VCreateRepresentation(Scene * pScene, EntityPtr pEntity);

	//create its physics model
	AVOID VCreatePhysicalBody(IPhysics * pPhysics, EntityPtr pEntity);

	/// ***	Copy resource data *** ///
	EntityResource & VCopy();
};

class PlaneEntity : public Entity
{
public:
	PlaneEntity(EntityId id) : Entity(id) {}
};

class PlaneEntityRepresentation : public EntityRepresentation
{
public:
	//constructor
	PlaneEntityRepresentation(EntityRepresentationId id,	AWSTRING diffuseMaterialFile, AWSTRING normalMaterialFile,
															AWSTRING specularMaterialFile, AWSTRING heightMaterialFile, const float2 & f2Size);
};
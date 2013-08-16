#pragma once
//========================================================================
// BoxEntityResource.h
//
// This code is part of Minotower Games Engine 1.0v
//
// (c) Copyright 2012 Muralev Evgeny
//========================================================================

#include "Entities\EntityResource.h"

struct BoxEntityResource : public EntityResource
{
	/***	Box size	***/
	AREAL	m_r32X;
	AREAL	m_r32Y;
	AREAL	m_r32Z;

	BoxEntityResource() : EntityResource()
	{
	}

	AVOID VLoad(const ASTRING & filename);

	~BoxEntityResource() {}

	//create entity
	EntityPtr	VCreateEntity(Game * pGame);

	//create its representation
	EntityRepresentation* VCreateRepresentation(Scene * pScene, EntityPtr pEntity);

	//create its physics model
	AVOID VCreatePhysicalBody(IPhysics * pPhysics, EntityPtr pEntity);
	
	/// ***	Copy resource data *** ///
	EntityResource & VCopy();
};
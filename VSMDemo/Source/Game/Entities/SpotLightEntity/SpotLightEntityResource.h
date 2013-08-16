#pragma once
//========================================================================
// SpotLightEntityResource.h
//
// This code is part of Minotower Games Engine 1.0v
//
// (c) Copyright 2012 Muralev Evgeny
//========================================================================

#include "Entities\EntityResource.h"
#include "../../Math/Source/Vectors.h"

struct SpotLightEntityResource : public EntityResource
{
	/*** Light properties ***/
	Vec		m_color;
	Vec		m_pos;
	Vec		m_dir;
	AREAL32	m_r32InnerRadius;
	AREAL32 m_r32OuterRadius;
	AREAL32 m_r32Range;


	SpotLightEntityResource(const Vec color, const Vec pos, const Vec dir, const AREAL r32InnerRadius, const AREAL r32OuterRadius, const AREAL r32Range)
		: EntityResource()
	{
		m_color = color;
		m_pos = pos;
		m_dir = dir;
		m_r32InnerRadius = r32InnerRadius;
		m_r32OuterRadius = r32OuterRadius;
		m_r32Range = r32Range;
	}

	~SpotLightEntityResource() {}

	//create entity
	EntityPtr	VCreateEntity(Game * pGame);

	//create its representation
	EntityRepresentation* VCreateRepresentation(Scene * pScene, EntityPtr pEntity);

	//create its physics model
	AVOID VCreatePhysicalBody(IPhysics * pPhysics, EntityPtr pEntity);

	/// ***	Copy resource data *** ///
	EntityResource & VCopy();
};
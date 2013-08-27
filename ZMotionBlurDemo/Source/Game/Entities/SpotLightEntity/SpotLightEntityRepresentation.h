#pragma once
//========================================================================
// SpotLightEntityRepresentation.h
//
// This code is part of Minotower Games Engine 1.0v
//
// (c) Copyright 2012 Muralev Evgeny
//========================================================================
#include "Representations\DynamicRepresentation.h"
#include "../../Math/Source/Vectors.h"

class SpotLightEntityRepresentation : public DynamicRepresentation
{
public:
	//constructor
	SpotLightEntityRepresentation(EntityRepresentationId id, const Vec color, const Vec pos, const Vec dir, const AREAL r32InnerRadius, const AREAL r32OuterRadius, const AREAL32 r32Range);
	AVOID SetDirection(const Vec & dir);
};
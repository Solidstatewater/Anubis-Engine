#pragma once
//========================================================================
// PointLightEntityRepresentation.h
//
// This code is part of Minotower Games Engine 1.0v
//
// (c) Copyright 2012 Muralev Evgeny
//========================================================================

#include "Representations\DynamicRepresentation.h"
#include "../../Math/Source/Vectors.h"

class PointLightEntityRepresentation : public DynamicRepresentation
{
public:
	//constructor
	PointLightEntityRepresentation(EntityRepresentationId id, const Vec color, const Vec pos, const AREAL32 r32Range);
};
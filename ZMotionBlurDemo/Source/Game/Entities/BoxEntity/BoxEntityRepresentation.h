#pragma once
//========================================================================
// BoxEntityRepresentation.h
//
// This code is part of Minotower Games Engine 1.0v
//
// (c) Copyright 2012 Muralev Evgeny
//========================================================================

#include "Representations\DynamicRepresentation.h"

class BoxEntityRepresentation : public DynamicRepresentation
{
public:
	//constructor
	BoxEntityRepresentation(EntityRepresentationId id, AWSTRING diffuseMaterialFile, AWSTRING normalMaterialFile,
							AWSTRING specularMaterialFile, AWSTRING heightMaterialFile, const float3 & f3Size);
};
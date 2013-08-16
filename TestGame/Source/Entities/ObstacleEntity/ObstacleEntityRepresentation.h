#pragma once
//========================================================================
// ObstacleEntityRepresentation.h
//
// This code is part of Minotower Games Engine 1.0v
//
// (c) Copyright 2012 Muralev Evgeny
//========================================================================

#include "Representations\DynamicRepresentation.h"

class ObstacleEntityRepresentation : public DynamicRepresentation
{
public:
	//constructor
	ObstacleEntityRepresentation(EntityRepresentationId id, const ASTRING resourceMesh);
};
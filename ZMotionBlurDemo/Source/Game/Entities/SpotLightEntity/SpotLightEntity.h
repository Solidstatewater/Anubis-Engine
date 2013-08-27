#pragma once
//========================================================================
// SpotLightEntity.h
//
// This code is part of Minotower Games Engine 1.0v
//
// (c) Copyright 2012 Muralev Evgeny
//========================================================================

#include "Entities\Entity.h"

class SpotLightEntity : public Entity
{
public:
	SpotLightEntity(EntityId id) : Entity(id) {}
	AVOID SetDirection(const Vec & dir);
};
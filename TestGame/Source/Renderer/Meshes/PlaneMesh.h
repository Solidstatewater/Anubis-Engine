#pragma once
//========================================================================
// PlaneMesh.h
//
// This code is part of Minotower Games Engine 1.0v
//
// (c) Copyright 2012 Muralev Evgeny
//========================================================================

#include "Meshes\Meshes.h"

class PlaneMesh : public Mesh
{
public:
	PlaneMesh(ASTRING fileName, AREAL scalex = 1.0f, AREAL scaley = 1.0f);
	AVOID VPreRender(Renderer* pRenderer, const Mat4x4 & view, const Mat4x4 & viewprojection);
};
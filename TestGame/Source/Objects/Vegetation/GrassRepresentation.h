#pragma once
//========================================================================
// GrassRepresentation.h
//
// This code is part of Minotower Games Engine 1.0v
//
// (c) Copyright 2012 Muralev Evgeny
//========================================================================

#include "Representations\StaticRepresentation.h"
#include "Renderer_pch.h"
#include "../../Math/Source/Vectors.h"

namespace Anubis
{
	class GrassRepresentation : public StaticRepresentation
	{
	public:
		//constructor
		GrassRepresentation(AWSTRING diffuseMaterialFile, AWSTRING maskMaterialFile, 
			AWSTRING specularMaterialFile,
			const float3 & f3Size, const Vec & pos, const AREAL halfX, const AREAL halfY,
			const AINT32 num);
	};
}; //Anubis
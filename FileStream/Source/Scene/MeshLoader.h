#pragma once
//========================================================================
// MeshLoader.h
//
// This code is part of Minotower Games Engine 1.0v
//
// (c) Copyright 2012 Muralev Evgeny
//========================================================================

#include "Other\Material.h"
#include "..\..\Math\Source\Vectors.h"

namespace Anubis
{
	struct MeshMaterialData
	{
		MeshMaterialData(shared_ptr<Material> _material, UINT _startIndex, UINT _endIndex)
		{
			material = _material;
			startIndex = _startIndex;
			endIndex = _endIndex;
		}

		~MeshMaterialData() { /*no need to clean as shared pointer is used*/ }

		shared_ptr<Material>	material;
		UINT					startIndex; //apply material for indices in this range
		UINT					endIndex;
	};

	struct MaterialKeeper
	{
		std::vector<MeshMaterialData>	materials;
	};

	class MeshLoader
	{
		static ABOOL LoadObj(const AWSTRING fileName, float3** pVertices,
			float2** pTexCoords, float3** pNormals, MaterialKeeper** pMaterials,
			AUINT32 & numSubsets);
	};
}; //Anubis
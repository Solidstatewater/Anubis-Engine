#include "TestGame_pch.h"
#include "GrassRepresentation.h"

#include "Grass\GrassPack.h"

using namespace Anubis;

GrassRepresentation::GrassRepresentation(AWSTRING diffuseMaterialFile,
		AWSTRING maskMaterialFile, AWSTRING specularMaterialFile, const float3 & f3Size,
		const Vec & pos, const AREAL halfX, const AREAL halfY, const AINT32 num)
{
	GrassPack* pGrassMesh = new GrassPack(pos, halfX, halfY, num);

	pGrassMesh->SetDiffuse(diffuseMaterialFile);
	pGrassMesh->SetMask(maskMaterialFile);
	pGrassMesh->SetSpecular(specularMaterialFile);

	//pGrassMesh->SetSize(f3Size);

	shared_ptr<GrassPack> pGrassMeshPtr = make_shared<GrassPack>(*pGrassMesh);
	m_pMesh = pGrassMeshPtr;
} 
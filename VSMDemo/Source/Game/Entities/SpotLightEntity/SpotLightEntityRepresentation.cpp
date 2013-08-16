#include "VSMDemo_pch.h"
#include "../../../Renderer/Meshes/BoxMesh.h"
#include "Lights\SpotLight.h"
#include "SpotLightEntityRepresentation.h"

SpotLightEntityRepresentation::SpotLightEntityRepresentation(EntityRepresentationId id, const Vec color, const Vec pos, const Vec dir,
							const AREAL r32InnerRadius, const AREAL r32OuterRadius, const AREAL32 r32Range): DynamicRepresentation(id, float3(0.0f, 0.0f, 0.0f))
{
	SpotLight* pLight = new SpotLight(color, pos, Normalize(dir), r32Range, r32InnerRadius, r32OuterRadius);
	shared_ptr<SpotLight> pSpotLightPtr = make_shared<SpotLight>(*pLight);
	m_lights.push_back(pSpotLightPtr);
}

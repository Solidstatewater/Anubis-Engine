#include "TestGame_pch.h"
#include "PointLightEntityRepresentation.h"

#include "../../Renderer/Meshes/BoxMesh.h"
#include "Lights\PointLight.h"
#include "Lights\SpotLight.h"

PointLightEntityRepresentation::PointLightEntityRepresentation(EntityRepresentationId id, const Vec color, const Vec pos, const AREAL32 r32Range)
							: DynamicRepresentation(id, float3(0.0f, 0.0f, 0.0f))
{
	PointLight* pLight = new PointLight(color, pos, r32Range);
	shared_ptr<PointLight> pPointLightPtr = make_shared<PointLight>(*pLight);
//	SpotLight* pLight = new SpotLight(color, pos, Normalize(Vector(0.5, 0.5, 1, 0)), 20, (AREAL)Pi / 16.0f, (AREAL)Pi / 4.0f);
	//shared_ptr<SpotLight> pPointLightPtr = make_shared<SpotLight>(*pLight);
	m_lights.push_back(pPointLightPtr);
}

//const Vec& color, const Vec& pos, const Vec& dir,
//					AREAL32 r32InnerAngleInRadians, AREAL32 r32OuterAngleInRadians
#include "ZMotionBlur_pch.h"
#include "MotionBlurDemo.h"

#include "../../../Math/Source/Geometry/Frustum.h"
#include "Controllers.h"

using namespace Anubis;

ABOOL MotionBlurDemo::VInitialize()
{
	/*** Initialize player view ***/
	shared_ptr<PlayerView> pView = static_pointer_cast<PlayerView>(VAddGameView(make_shared<PlayerView>(PlayerView())));

	//Add camera
	Frustum & frustum = Frustum();
	//frustum.Init(Pi / 2.0, 1280.0 / 720.0, 0.1, 100.0);
	frustum.Init( 0.25f * 3.14f, (AREAL)SCREEN_WIDTH / (AREAL)SCREEN_HEIGHT, 1.0f, VIEW_DISTANCE);
	Viewport & viewport = Viewport();
	viewport.Init(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0.01f, 500.0f);
	/*CameraPtr pCamera = make_shared<Camera>(Camera(frustum, Vector(0.0f, 0.0f, -70.0, 1.0f),
		Vector(0.0f, 0.0f, 1.0f, 0.0f),
		Vector(0.0f, 1.0f, 0.0f, 0.0f),
		0.0f, 0.0, 0.0, viewport)); */
	CameraPtr pCamera = make_shared<Camera>(Camera(frustum, Vector(0.0f, 4.0f, -7.0, 1.0f),
		Vector(0.0f, 0.0f, 1.0f, 0.0f),
		Vector(0.0f, 1.0f, 0.0f, 0.0f),
		0.0f, 0.8, 0.0, viewport));

	pView->VSetCamera(pCamera);

	//Input controllers
	FreeCameraController* m_pFreeCameraController = new FreeCameraController();
	pView->SetMouseHandler(m_pFreeCameraController);
	pView->SetKeyboardHandler(m_pFreeCameraController);
	pView->SetController(m_pFreeCameraController);

	//call base class initialization
	return Game::VInitialize();
}
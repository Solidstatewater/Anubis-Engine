#include <Windows.h>
#include <DXUT.h>
#include "../Graphics/Source/Graphics_pch.h"
#include "../Graphics/Source/Final/Resources/Textures/Texture2D.h"

#include "../Renderer/Source/Renderer.h"

using namespace Anubis;

void CALLBACK TestTexture2D(double fTime, float fElapsedTime, void* pUserContext)
{
	Texture2DParams params;
	params.Init(100,
						100,
						1,
						TEX_R32G32B32A32_FLOAT,
						true,
						false,
						false,
						false,
						1,
						0,
						1,
						true,
						false,
						false); 

	Texture2D texture;
	//texture.Create(&params);
};

INT WINAPI GraphicsTest(HINSTANCE hInstance, HINSTANCE hPrevInstance,
					LPWSTR lpCmdLine,
					int	   nCmdShow)
{	

	//show cursor when in fullscreen
	DXUTSetCursorSettings(true, true);

	DXUTSetCallbackFrameMove( (LPDXUTCALLBACKFRAMEMOVE)TestTexture2D );

	//run DXUT framework
	DXUTMainLoop();
	DXUTShutdown();

	return 0;
};
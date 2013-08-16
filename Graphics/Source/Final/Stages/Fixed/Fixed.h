#pragma once
//========================================================================
// Graphics.h
//
// This code is part of Minotower Games Engine 1.0v
//
// (c) Copyright 2012 Muralev Evgeny
//========================================================================

#include "../../../../DX11/Stages/Fixed/InputAssemblerDX11.h"
#include "../../../../DX11/Stages/Fixed/OutputMergerDX11.h"
#include "../../../../DX11/Stages/Fixed/RasterizerDX11.h"

namespace Anubis
{
	class InputAssembler : public
		#ifdef ADX11_API
			InputAssemblerStateDX11
		#endif
	{
	};

	/*class OutputMerger : public
		#ifdef ADX11_API
			OutputMergerDX11
		#endif
	{
	}; */

	class Viewport : public
		#ifdef ADX11_API
			ViewportDX11
		#endif
	{
	};
}; //Anubis
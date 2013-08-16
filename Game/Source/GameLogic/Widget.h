#pragma once
//========================================================================
// Widget.h
//
// This code is part of Minotower Games Engine 1.0v
//
// (c) Copyright 2012 Muralev Evgeny
//========================================================================

#include "Anubis.h"

namespace Anubis
{
	class Widget
	{
	public:
		AVIRTUAL AVOID		VUpdate(AUINT32 const deltaMilliseconds)								= 0;
		AVIRTUAL AVOID		VRender(Renderer* pRenderer, AREAL64 r64Time, AREAL64 r64ElapsedTime)	= 0;
		AVIRTUAL ABOOL		VMsgProc(SystemMessage * msg)											= 0;

		//To order widgets on the screen
		AVIRTUAL AVOID		VSetZ(AUINT8 z)															= 0;
		AVIRTUAL AUINT8		VGetZ() const															= 0;

		AVIRTUAL AVOID		VSetVisible(const ABOOL visible)										= 0;
		AVIRTUAL ABOOL		VGetVisible() const														= 0;

		AVIRTUAL ABOOL const operator<(const Widget & widget)
		{
			return VGetZ() < widget.VGetZ();
		}
	};

	typedef shared_ptr<Widget>	WidgetPtr;
	typedef vector<WidgetPtr>	Widgets;
};
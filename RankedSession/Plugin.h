#pragma once

#pragma comment( lib, "pluginsdk.lib" )
#include "Renderer.h"

namespace RankedSession
{
	class Plugin : BakkesMod::Plugin::BakkesModPlugin
	{
	public:
		virtual void onLoad();
		virtual void onUnload();

	private:
		Renderer renderer;
	};
}
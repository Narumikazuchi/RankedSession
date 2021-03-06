#pragma once

#pragma comment( lib, "pluginsdk.lib" )
#include "Renderer.h"

namespace RankedSession
{
	class Plugin : BakkesMod::Plugin::BakkesModPlugin
	{
	public:
		Plugin();

		void ResetColors();
		virtual void onLoad();
		virtual void onUnload();
		void Render(CanvasWrapper canvas);
		void Event_GameBegin(std::string eventName);
		void Event_GameConcluded(std::string eventName);
		void Event_GameLeft(std::string eventName);

	private:
		bool drawStats;
		bool drawResults;
		std::shared_ptr<Renderer> renderer;
		std::shared_ptr<GameResultViewer> resultViewer;
		std::shared_ptr<StatTracker> statTracker;

		void SyncTracker(const int retries);
		void UpdateTrackers(const int retries, const RankedPlaylist playlist);
	};
}
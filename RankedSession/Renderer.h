#pragma once

#include <memory>
#include "RankedData.h"
#include "GameResultViewer.h"
#include "StatTracker.h"

namespace RankedSession
{

	class Renderer
	{
	public:
		std::shared_ptr<int> posX;
		std::shared_ptr<int> posY;
		bool isInitialized;

		std::shared_ptr<LinearColor> colorBackground;
		std::shared_ptr<LinearColor> colorTitle;
		std::shared_ptr<LinearColor> colorLabel;
		std::shared_ptr<LinearColor> colorPositive;
		std::shared_ptr<LinearColor> colorNegative;

		Renderer();
		Renderer(GameWrapper* wrapper);
		void RenderSessionInfo(CanvasWrapper* canvas, StatTracker* tracker);
		void RenderAfterGame(CanvasWrapper* canvas, GameResultViewer* viewer, const RankedPlaylist playlist);
		void PreLoadImages(GameWrapper* warpper);

	private:
		GameWrapper* wrapper;

		void SetColorByValue(CanvasWrapper* canvas, const float value);
		void RenderBox(CanvasWrapper* canvas, Stats* stats, const Vector2 position, const std::string playlist);
	};
}
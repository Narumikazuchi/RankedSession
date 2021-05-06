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

		std::shared_ptr<LinearColor> colorBackground;
		std::shared_ptr<LinearColor> colorTitle;
		std::shared_ptr<LinearColor> colorLabel;
		std::shared_ptr<LinearColor> colorPositive;
		std::shared_ptr<LinearColor> colorNegative;

		Renderer();
		void RenderSessionInfo(CanvasWrapper* canvas, StatTracker& tracker, RankedPlaylist playlist);
		void RenderAfterGame(CanvasWrapper* canvas, GameResultViewer& viewer, RankedPlaylist playlist);

	private:
		void SetColorByValue(CanvasWrapper* canvas, float value);
	};
}
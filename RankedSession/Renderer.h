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
		void RenderSessionInfo(CanvasWrapper* canvas, StatTracker* tracker);
		void RenderAfterGame(CanvasWrapper* canvas, GameResultViewer* viewer, const RankedPlaylist playlist);

	private:
		void SetColorByValue(CanvasWrapper* canvas, const float value);
		void RenderBox(CanvasWrapper* canvas, Stats stats, const Vector2 position, const std::string playlist);
	};
}
#pragma once

#include "bakkesmod/plugin/bakkesmodplugin.h"

namespace RankedSession
{
	class GameResultViewer
	{
	public:
		bool shouldDraw;

		std::shared_ptr<LinearColor> colorPrevious;
		std::shared_ptr<LinearColor> colorCurrent;
		std::shared_ptr<LinearColor> colorNext;
		std::string tierPrevious;
		std::string tierCurrent;
		std::string tierNext;
		std::string differencePrevious;
		std::string differenceNext;
		int ratingPrevious;
		int ratingCurrent;
		int ratingNext;

		bool IsPlacement();
	};
}
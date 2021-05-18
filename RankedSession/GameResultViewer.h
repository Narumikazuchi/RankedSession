#pragma once

#include "bakkesmod/plugin/bakkesmodplugin.h"
#include "RatingData.h"
#include "RatingTracker.h"

namespace RankedSession
{
	class GameResultViewer
	{
	public:
		bool isInitialized;

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

		GameResultViewer(std::shared_ptr<GameWrapper> wrapper);
		bool IsPlacement(const RankedPlaylist playlist);
		RatingUpdateResult Update(const RankedPlaylist playlist);

	private:
		std::shared_ptr<GameWrapper> wrapper;
		std::map<RankedPlaylist, bool> placement;

		DivisionData GetDivisionData(const RankedPlaylist playlist, const Rank rank, const int division);
		void SetRankColor(LinearColor* color, int tier);
		void SetDifferenceString(std::string* ptr, int difference);
		int GetRatingLowerBound(const RankedPlaylist playlist, const int tier, const int division);
		int GetRatingUpperBound(const RankedPlaylist playlist, const int tier, const int division);
	};
}
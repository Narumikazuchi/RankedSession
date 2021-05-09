#pragma once

#include <map>
#include "RatingTracker.h"

namespace RankedSession
{
	enum class StreamFormatStyle
	{
		InitialRating = 0,
		CurrentRating = 1,
		LastGameDifference = 2,
		InitialDifference = 3
	};

	struct Stats
	{
	public:
		std::shared_ptr<RatingTracker> rating;
		int wins;
		int losses;
		int streak;
		bool wonLast;

		Stats();
		void FormatStream(std::stringstream& stream, const StreamFormatStyle style);
	};

	class StatTracker
	{
	public:
		std::map<RankedPlaylist, Stats*> stats;
		bool isInitialized;

		StatTracker(std::shared_ptr<GameWrapper> wrapper);
		RatingUpdateResult Sync();
		RatingUpdateResult Update(const RankedPlaylist playlist);

	private:
		std::shared_ptr<GameWrapper> wrapper;
		bool synced;
	};
}
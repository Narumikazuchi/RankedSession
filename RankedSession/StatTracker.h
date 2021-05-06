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
		RatingTracker rating;
		int wins;
		int losses;
		int streak;
		bool wonLast;

		Stats();
		Stats(MMRWrapper* wrapper, const UniqueIDWrapper id, const RankedPlaylist playlist);
		void FormatStream(std::stringstream& stream, const StreamFormatStyle style);
	};

	class StatTracker
	{
	public:
		std::map<RankedPlaylist, Stats> stats;

		StatTracker(GameWrapper* wrapper);
		void Update(GameWrapper* wrapper, const RankedPlaylist playlist);
	};
}
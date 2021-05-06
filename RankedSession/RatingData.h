#pragma once

#include <vector>
#include "RankedData.h"

namespace RankedSession
{
	// The lower and upper mmr boundary of a division (e.g. silver 2 division 2)
	struct DivisionData
	{
	public:
		int lower;
		int higher;
	};

	// The mmr boundaries for the 4 divisions of a tier (e.g. gold 3)
	struct TierData
	{
	public:
		std::vector<DivisionData> divisions;
	};

	// The mmr boundaries for all tiers in a playlist.
	struct RatingData
	{
	public:
		std::vector<TierData> tiers;
	};

	extern std::map<RankedPlaylist, RatingData&> PlaylistRatingDatabase;
}
#pragma once

#include "RankedData.h"

namespace RankedSession
{
	enum class RatingRequestResult
	{
		INVALID_VALUE = 0,
		NOT_SYNCED = 1 << 1,
		SAME_VALUE = 1 << 2,
		SUCCESS_WIN = 1 << 3,
		SUCCESS_LOSE = 1 << 4
	};

	class RatingTracker
	{
	public:
		float initialRating;
		float beforeRating;
		float currentRating;
		Rank initialRank;
		Rank beforeRank;
		Rank currentRank;

		RatingRequestResult RequestUpdate(GameWrapper* wrapper, const RankedPlaylist playlist);
		float GetSessionDifference();
		float GetLastGameDifference();
	};
}
#include "bakkesmod/wrappers/MMRWrapper.h"
#include "RatingTracker.h"

namespace RankedSession
{
	RatingRequestResult RatingTracker::RequestUpdate(GameWrapper* wrapper, const RankedPlaylist playlist)
	{
		return RatingRequestResult::INVALID_VALUE;
	}

	float RatingTracker::GetSessionDifference()
	{
		return this->currentRating - this->initialRating;
	}

	float RatingTracker::GetLastGameDifference()
	{
		return this->currentRating - this->beforeRating;
	}
}
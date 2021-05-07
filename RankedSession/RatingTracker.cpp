#include "bakkesmod/wrappers/MMRWrapper.h"
#include "RatingTracker.h"

namespace RankedSession
{
	RatingRequestResult RatingTracker::RequestUpdate(GameWrapper* wrapper, const RankedPlaylist playlist)
	{
		if (wrapper == nullptr)
		{
			return RatingRequestResult::NOT_SYNCED;
		}

		UniqueIDWrapper id = wrapper->GetUniqueID();
		MMRWrapper mmrWrapper = wrapper->GetMMRWrapper();
		float mmr = mmrWrapper.GetPlayerMMR(id, (int)playlist);
		if (this->currentRating == mmr)
		{
			return RatingRequestResult::SAME_VALUE;
		}
		if (std::floor(mmr) >= 100.0f &&
			std::floor(mmr) < 101.0f)
		{
			return RatingRequestResult::INVALID_VALUE;
		}

		this->beforeRating = this->currentRating;
		this->beforeRank = this->currentRank;

		Rank rank = (Rank)mmrWrapper.GetPlayerRank(id, (int)playlist).Tier;
		this->currentRating = mmr;
		this->currentRank = rank;
		return this->currentRating > this->beforeRating ? RatingRequestResult::SUCCESS_WIN : RatingRequestResult::SUCCESS_LOSE;
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
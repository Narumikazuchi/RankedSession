#include "bakkesmod/wrappers/MMRWrapper.h"
#include "Logger.h"
#include "RatingTracker.h"

namespace RankedSession
{
	RatingRequestResult RatingTracker::RequestUpdate(GameWrapper* wrapper, const RankedPlaylist playlist)
	{
		if (wrapper == nullptr)
		{
			Log("RatingTracker::RequestUpdate(GameWrapper*, const RankedPlaylist) - GameWrapper pointer was null");
			return RatingRequestResult::NOT_SYNCED;
		}

		if (!IsPlaylistValid(playlist))
		{
			return RatingRequestResult::INVALID_VALUE;
		}

		UniqueIDWrapper id = wrapper->GetUniqueID();
		MMRWrapper mmrWrapper = wrapper->GetMMRWrapper();

		if (mmrWrapper.IsSyncing(id) ||
			!mmrWrapper.IsSynced(id, (int)playlist))
		{
			return RatingRequestResult::NOT_SYNCED;
		}

		float mmr = mmrWrapper.GetPlayerMMR(id, (int)playlist);
		Rank rank = (Rank)mmrWrapper.GetPlayerRank(id, (int)playlist).Tier;
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
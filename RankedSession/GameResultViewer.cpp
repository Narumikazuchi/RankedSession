#include "GameResultViewer.h"
#include "Logger.h"

namespace RankedSession
{
	GameResultViewer::GameResultViewer(std::shared_ptr<GameWrapper> wrapper)
	{
		this->colorPrevious = std::make_shared<LinearColor>();
		this->colorCurrent = std::make_shared<LinearColor>();
		this->colorNext = std::make_shared<LinearColor>();
		this->ratingPrevious = 0;
		this->ratingCurrent = 0;
		this->ratingNext = 0;
		
		for (RankedPlaylist playlist : AvailablePlaylists)
		{
			this->placement.insert(std::pair<RankedPlaylist, bool>(playlist, true));
		}

		if (wrapper.get() == nullptr)
		{
			this->isInitialized = false;
			this->wrapper = nullptr;
			return;
		}
		this->isInitialized = true;
		this->wrapper = wrapper;
	}

	bool GameResultViewer::IsPlacement(const RankedPlaylist playlist)
	{
		if (!this->isInitialized ||
			!RankedSession::IsPlaylistValid(playlist))
		{
			return true;
		}

		return this->placement[playlist];
	}

	RatingUpdateResult GameResultViewer::Update(const RankedPlaylist playlist)
	{
		if (!this->isInitialized)
		{
			return RatingUpdateResult::NOT_INITIALIZED;
		}

		// Ranked Check
		MMRWrapper mmr = this->wrapper->GetMMRWrapper();
		if (!mmr.IsRanked((int)playlist) ||
			!IsPlaylistValid(playlist))
		{
			return RatingUpdateResult::INVALID_OPTION;
		}
		this->placement[playlist] = true;

		// Game Check
		ServerWrapper server = this->wrapper->GetOnlineGame();
		if (server.IsNull() ||
			!server.IsOnlineMultiplayer() ||
			this->wrapper->IsInReplay())
		{
			return RatingUpdateResult::INVALID_OPTION;
		}

		// Placement Check
		UniqueIDWrapper id = this->wrapper->GetUniqueID();
		if (mmr.IsSyncing(id) ||
			!mmr.IsSynced(id, (int)playlist))
		{
			return RatingUpdateResult::NOT_SYNCED;
		}
		this->ratingCurrent = (int)(round(mmr.GetPlayerMMR(id, (int)playlist)));
		SkillRank rank = mmr.GetPlayerRank(id, (int)playlist);
		int userTier = rank.Tier;
		int userDivision = rank.Division;
		if (rank.Tier <= 0)
		{
			return RatingUpdateResult::INVALID_OPTION;
		}

		this->placement[playlist] = false;
		// Tier Data
		this->tierCurrent = GetRankName((Rank)userTier, userDivision);
		this->SetRankColor(this->colorCurrent.get(), userTier);

		int nextTier, prevTier;
		int nextDiv, prevDiv;
		if (userDivision == 0)
		{
			nextTier = userTier;
			prevTier = userTier - 1;
			nextDiv = userDivision + 1;
			prevDiv = 3;
		}
		else if (userDivision == 3)
		{
			nextTier = userTier + 1;
			prevTier = userTier;
			nextDiv = 1;
			prevDiv = userDivision - 1;
		}
		else
		{
			nextTier = userTier;
			prevTier = userTier;
			nextDiv = userDivision + 1;
			prevDiv = userDivision - 1;
		}

		int diff = 0;
		// Next Higher Tier
		this->ratingNext = this->GetRatingLowerBound(playlist, nextTier, nextDiv);
		diff = this->ratingNext - this->ratingCurrent;
		this->SetDifferenceString(&this->differenceNext, diff);
		this->tierNext = GetRankName((Rank)nextTier, nextDiv);
		this->SetRankColor(this->colorNext.get(), nextTier);
		// Next Lower Tier
		this->ratingPrevious = this->GetRatingLowerBound(playlist, prevTier, prevDiv);
		diff = this->ratingPrevious - this->ratingCurrent;
		this->SetDifferenceString(&this->differencePrevious, diff);
		this->tierPrevious = GetRankName((Rank)prevTier, prevDiv);
		this->SetRankColor(this->colorPrevious.get(), prevTier);
		return RatingUpdateResult::SUCCESS;
	}

	//
	// Private
	//

	DivisionData GameResultViewer::GetDivisionData(const RankedPlaylist playlist, const Rank rank, const int division)
	{
		std::map<RankedPlaylist, RatingData&>::iterator playlistSearch = PlaylistRatingDatabase.find(playlist);
		if (playlistSearch != PlaylistRatingDatabase.end())
		{
			RatingData playlist = playlistSearch->second;
			if (playlist.tiers.size() > (int)rank)
			{
				TierData tier = playlist.tiers[(int)rank];
				if (tier.divisions.size() > division)
				{
					return tier.divisions[division];
				}
			}
		}
		return DivisionData();
	}
	
	void GameResultViewer::SetRankColor(LinearColor* color, int tier)
	{
		if (color == nullptr)
		{
			return;
		}

		Rank rank = (Rank)tier;
		if (rank < Rank::Unranked ||
			rank > Rank::SupersonicLegend)
		{
			color->R = 0;
			color->G = 0;
			color->B = 0;
			return;
		}
		LinearColor source = RankInfoDatabase[rank].color;
		color->R = source.R;
		color->G = source.G;
		color->B = source.B;
	}

	void GameResultViewer::SetDifferenceString(std::string* ptr, int difference)
	{
		if (ptr == nullptr)
		{
			return;
		}

		if (difference >= 0)
		{
			difference = abs(difference);
			*ptr = " (+ " + std::to_string(difference) + ")";
		}
		else
		{
			difference = abs(difference);
			*ptr = " (- " + std::to_string(difference) + ")";
		}
	}

	int GameResultViewer::GetRatingLowerBound(const RankedPlaylist playlist, const int tier, const int division)
	{
		// Rank enum is in ascending order, while the SkillRank.Tier is in descending order
		int rankIndex[] = { 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0 };
		int rank = rankIndex[tier];

		DivisionData data = this->GetDivisionData(playlist, (Rank)rank, division);
		return data.lower;
	}

	int GameResultViewer::GetRatingUpperBound(const RankedPlaylist playlist, const int tier, const int division)
	{
		// Rank enum is in ascending order, while the SkillRank.Tier is in descending order
		int rankIndex[] = { 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0 };
		int rank = rankIndex[tier];

		DivisionData data = this->GetDivisionData(playlist, (Rank)rank, division);
		return data.higher;
	}
}
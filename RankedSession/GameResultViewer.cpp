#include "GameResultViewer.h"

namespace RankedSession
{
	GameResultViewer::GameResultViewer(GameWrapper* wrapper)
	{
		if (wrapper == nullptr)
		{
			this->isInitialized = false;
		}
		else
		{
			this->isInitialized = true;
		}
		this->wrapper = wrapper;
		this->shouldDraw = false;
		this->colorPrevious = std::make_shared<LinearColor>(LinearColor());
		this->colorCurrent = std::make_shared<LinearColor>(LinearColor());
		this->colorNext = std::make_shared<LinearColor>(LinearColor());
		this->ratingPrevious = 0;
		this->ratingCurrent = 0;
		this->ratingNext = 0;
	}

	bool GameResultViewer::IsPlacement(const RankedPlaylist playlist)
	{
		if (!this->isInitialized)
		{
			return true;
		}
		MMRWrapper mmr = this->wrapper->GetMMRWrapper();
		UniqueIDWrapper id = this->wrapper->GetUniqueID();
		SkillRank rank = mmr.GetPlayerRank(id, (int)playlist);
		return rank.Tier <= 0;
	}

	void GameResultViewer::Update(const RankedPlaylist playlist)
	{
		if (!this->isInitialized)
		{
			return;
		}

		// Ranked Check
		MMRWrapper mmr = this->wrapper->GetMMRWrapper();
		if (!mmr.IsRanked((int)playlist))
		{
			this->shouldDraw = false;
			return;
		}

		// Game Check
		ServerWrapper server = this->wrapper->GetOnlineGame();
		if (server.IsNull() ||
			!server.IsOnlineMultiplayer() ||
			this->wrapper->IsInReplay())
		{
			this->shouldDraw = false;
			return;
		}

		// Placement Check
		UniqueIDWrapper id = this->wrapper->GetUniqueID();
		this->ratingCurrent = (int)(round(mmr.GetPlayerMMR(id, (int)playlist)));
		SkillRank rank = mmr.GetPlayerRank(id, (int)playlist);
		int userTier = rank.Tier;
		int userDivision = rank.Division;
		if (rank.Tier <= 0)
		{
			this->shouldDraw = false;
			return;
		}

		// Tier Data
		this->shouldDraw = true;
		this->tierCurrent = this->GetRankName(userTier, userDivision);
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
		this->ratingNext = GetRatingLowerBound(playlist, nextTier, nextDiv);
		diff = this->ratingNext - this->ratingCurrent;
		this->SetDifferenceString(&this->differenceNext, diff);
		this->tierNext = this->GetRankName(nextTier, nextDiv);
		this->SetRankColor(this->colorNext.get(), nextTier);
		// Next Lower Tier
		this->ratingPrevious = GetRatingLowerBound(playlist, prevTier, prevDiv);
		diff = this->ratingPrevious - this->ratingCurrent;
		this->SetDifferenceString(&this->differencePrevious, diff);
		this->tierPrevious = this->GetRankName(prevTier, prevDiv);
		this->SetRankColor(this->colorPrevious.get(), prevTier);
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

	std::string GameResultViewer::GetRankName(const int tier, const int division)
	{
		if (tier < 0 ||
			tier > 22)
		{
			return "ERROR";
		}

		Rank realRank = (Rank)(tier);
		std::string rankName = RankInfoDatabase[realRank].name;
		if (realRank != Rank::Unranked &&
			realRank != Rank::SuperSonicLegend)
		{
			rankName += " Div " + std::to_string(division + 1);
		}

		return rankName;
	}
	
	void GameResultViewer::SetRankColor(LinearColor* color, int tier)
	{
		if (color == nullptr)
		{
			return;
		}

		Rank rank = (Rank)tier;
		if (rank < Rank::Unranked ||
			rank > Rank::SuperSonicLegend)
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
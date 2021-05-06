#include <iomanip>
#include "StatTracker.h"

namespace RankedSession
{
	Stats::Stats()
	{
		this->losses = 0;
		this->wins = 0;
		this->streak = 0;
		this->wonLast = false;
		this->rating = RatingTracker();
		this->rating.initialRating = 0;
		this->rating.beforeRating = 0;
		this->rating.currentRating = 0;
		this->rating.initialRank = Rank::Unranked;
		this->rating.beforeRank = Rank::Unranked;
		this->rating.currentRank = Rank::Unranked;
	}

	Stats::Stats(MMRWrapper* wrapper, const UniqueIDWrapper id, const RankedPlaylist playlist)
	{
		float initial = wrapper->GetPlayerMMR(id, (int)playlist);
		Rank rank = (Rank)wrapper->GetPlayerRank(id, (int)playlist).Tier;
		this->rating = RatingTracker();
		this->rating.initialRating = initial;
		this->rating.beforeRating = initial;
		this->rating.currentRating = initial;
		this->rating.initialRank = rank;
		this->rating.beforeRank = rank;
		this->rating.currentRank = rank;
		this->losses = 0;
		this->wins = 0;
		this->streak = 0;
		this->wonLast = false;
	}

	void Stats::FormatStream(std::stringstream& stream, const StreamFormatStyle style)
	{
		switch (style)
		{
		case StreamFormatStyle::InitialRating:
			stream << std::setprecision(2) << std::showpoint << std::fixed << this->rating.initialRating;
			break;
		case StreamFormatStyle::CurrentRating:
			stream << std::setprecision(2) << std::showpoint << std::fixed << this->rating.currentRating;
			break;
		case StreamFormatStyle::LastGameDifference:
			this->FormatStream(stream, StreamFormatStyle::CurrentRating);
			stream << (this->wonLast ? " (+" : " (") << std::setprecision(2) << std::showpoint << std::fixed << this->rating.GetLastGameDifference() << ")";
			break;
		case StreamFormatStyle::InitialDifference:
			this->FormatStream(stream, StreamFormatStyle::InitialRating);
			stream << (this->rating.currentRating > this->rating.initialRating ? " (+" : " (") << std::setprecision(2) << std::showpoint << std::fixed << this->rating.GetSessionDifference() << ")";
			break;
		}
	}

	StatTracker::StatTracker(GameWrapper* wrapper)
	{
		UniqueIDWrapper id = wrapper->GetUniqueID();
		MMRWrapper mmr = wrapper->GetMMRWrapper();
		for (const RankedPlaylist playlist : AvailablePlaylists)
		{
			this->stats.insert(std::pair<RankedPlaylist, Stats>(playlist, Stats(&mmr, id, playlist)));
		}
	}

	void StatTracker::Update(GameWrapper* wrapper, const RankedPlaylist playlist)
	{
		Stats stats = this->stats[playlist];
		UniqueIDWrapper id = wrapper->GetUniqueID();
		MMRWrapper mmr = wrapper->GetMMRWrapper();
		RatingRequestResult result = stats.rating.RequestUpdate(wrapper, playlist);
		if (result == RatingRequestResult::SUCCESS_WIN)
		{
			stats.wins++;
			if (stats.wonLast)
			{
				stats.streak++;
			}
			else
			{
				stats.streak = 1;
			}
			stats.wonLast = true;
		}
		else if (result == RatingRequestResult::SUCCESS_LOSE)
		{
			stats.losses++;
			if (stats.wonLast)
			{
				stats.streak = 1;
			}
			else
			{
				stats.streak++;
			}
			stats.wonLast = false;
		}
	}
}
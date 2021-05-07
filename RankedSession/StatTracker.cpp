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
		this->rating = new RatingTracker();
		this->rating->initialRating = 0;
		this->rating->beforeRating = 0;
		this->rating->currentRating = 0;
		this->rating->initialRank = Rank::Unranked;
		this->rating->beforeRank = Rank::Unranked;
		this->rating->currentRank = Rank::Unranked;
	}

	Stats::Stats(MMRWrapper* wrapper, const UniqueIDWrapper id, const RankedPlaylist playlist)
	{
		this->rating = new RatingTracker();
		this->rating->initialRating = 0.f;
		this->rating->beforeRating = 0.f;
		this->rating->currentRating = 0.f;
		this->rating->initialRank = Rank::Unranked;
		this->rating->beforeRank = Rank::Unranked;
		this->rating->currentRank = Rank::Unranked;
		this->losses = 0;
		this->wins = 0;
		this->streak = 0;
		this->wonLast = false;
		if (wrapper != nullptr)
		{
			this->rating->initialRating = this->rating->beforeRating = this->rating->currentRating = wrapper->GetPlayerMMR(id, (int)playlist);
			this->rating->initialRank = this->rating->beforeRank = this->rating->currentRank = (Rank)wrapper->GetPlayerRank(id, (int)playlist).Tier;
		}
	}

	void Stats::FormatStream(std::stringstream& stream, const StreamFormatStyle style)
	{
		switch (style)
		{
		case StreamFormatStyle::InitialRating:
			stream << std::setprecision(2) << std::showpoint << std::fixed << this->rating->initialRating;
			break;
		case StreamFormatStyle::CurrentRating:
			stream << std::setprecision(2) << std::showpoint << std::fixed << this->rating->currentRating;
			break;
		case StreamFormatStyle::LastGameDifference:
			this->FormatStream(stream, StreamFormatStyle::CurrentRating);
			stream << (this->wonLast ? " (+" : " (") << std::setprecision(2) << std::showpoint << std::fixed << this->rating->GetLastGameDifference() << ")";
			break;
		case StreamFormatStyle::InitialDifference:
			this->FormatStream(stream, StreamFormatStyle::InitialRating);
			stream << (this->rating->currentRating > this->rating->initialRating ? " (+" : " (") << std::setprecision(2) << std::showpoint << std::fixed << this->rating->GetSessionDifference() << ")";
			break;
		}
	}

	StatTracker::StatTracker(GameWrapper* wrapper)
	{
		if (wrapper == nullptr)
		{
			this->isInitialized = false;
			this->wrapper = nullptr;
			return;
		}
		this->wrapper = wrapper;
		this->isInitialized = true;
		UniqueIDWrapper id = this->wrapper->GetUniqueID();
		MMRWrapper mmr = this->wrapper->GetMMRWrapper();
		for (const RankedPlaylist playlist : AvailablePlaylists)
		{
			this->stats.insert(std::pair<RankedPlaylist, Stats*>(playlist, new Stats(&mmr, id, playlist)));
		}
	}

	void StatTracker::Update(const RankedPlaylist playlist)
	{
		if (!this->isInitialized)
		{
			return;
		}
		Stats* stats = this->stats[playlist];
		UniqueIDWrapper id = this->wrapper->GetUniqueID();
		MMRWrapper mmr = this->wrapper->GetMMRWrapper();
		RatingRequestResult result = stats->rating->RequestUpdate(this->wrapper, playlist);
		if (result == RatingRequestResult::SUCCESS_WIN)
		{
			stats->wins++;
			if (stats->wonLast)
			{
				stats->streak++;
			}
			else
			{
				stats->streak = 1;
			}
			stats->wonLast = true;
		}
		else if (result == RatingRequestResult::SUCCESS_LOSE)
		{
			stats->losses++;
			if (stats->wonLast)
			{
				stats->streak = 1;
			}
			else
			{
				stats->streak++;
			}
			stats->wonLast = false;
		}
	}
}
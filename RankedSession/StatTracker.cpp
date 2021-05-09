#include <iomanip>
#include "Logger.h"
#include "StatTracker.h"

namespace RankedSession
{
	Stats::Stats()
	{
		this->losses = 0;
		this->wins = 0;
		this->streak = 0;
		this->wonLast = false;
		this->rating = std::make_shared<RatingTracker>();
		this->rating->initialRating = 0;
		this->rating->beforeRating = 0;
		this->rating->currentRating = 0;
		this->rating->initialRank = Rank::Unranked;
		this->rating->beforeRank = Rank::Unranked;
		this->rating->currentRank = Rank::Unranked;
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

	StatTracker::StatTracker(std::shared_ptr<GameWrapper> wrapper)
	{
		if (wrapper.get() == nullptr)
		{
			this->isInitialized = false;
			this->wrapper = nullptr;
			return;
		}
		this->wrapper = wrapper;
		this->isInitialized = true;
		this->synced = false;
		UniqueIDWrapper id = this->wrapper->GetUniqueID();
		MMRWrapper mmr = this->wrapper->GetMMRWrapper();
		for (const RankedPlaylist playlist : AvailablePlaylists)
		{
			this->stats.insert(std::pair<RankedPlaylist, Stats*>(playlist, new Stats()));
		}
	}

	RatingUpdateResult StatTracker::Sync()
	{
		if (!this->isInitialized)
		{
			return RatingUpdateResult::NOT_INITIALIZED;
		}

		if (this->synced)
		{
			return RatingUpdateResult::SUCCESS;
		}

		MMRWrapper mmr = this->wrapper->GetMMRWrapper();
		UniqueIDWrapper id = this->wrapper->GetUniqueID();
		if (mmr.IsSyncing(id))
		{
			return RatingUpdateResult::NOT_SYNCED;
		}
		for (RankedPlaylist playlist : AvailablePlaylists)
		{
			if (!mmr.IsSynced(id, (int)playlist))
			{
				return RatingUpdateResult::NOT_SYNCED;
			}
		}

		for (RankedPlaylist playlist : AvailablePlaylists)
		{
			float rating = mmr.GetPlayerMMR(id, (int)playlist);
			Rank rank = (Rank)mmr.GetPlayerRank(id, (int)playlist).Tier;
			if (this->stats[playlist] == nullptr)
			{
				continue;
			}
			this->stats[playlist]->rating->initialRating = rating;
			this->stats[playlist]->rating->currentRating = rating;
			this->stats[playlist]->rating->beforeRating = rating;
			this->stats[playlist]->rating->initialRank = rank;
			this->stats[playlist]->rating->currentRank = rank;
			this->stats[playlist]->rating->beforeRank = rank;
			Log("StatTracker::Sync() - initial Rating for playlist " + GetPlaylistName(playlist) + " = " + std::to_string(this->stats[playlist]->rating->initialRating));
		}
		this->synced = true;
		return RatingUpdateResult::SUCCESS;
	}

	RatingUpdateResult StatTracker::Update(const RankedPlaylist playlist)
	{
		if (!this->isInitialized)
		{
			return RatingUpdateResult::NOT_INITIALIZED;
		}

		if (this->stats[playlist] == nullptr)
		{
			return RatingUpdateResult::INVALID_OPTION;
		}

		Stats* stats = this->stats[playlist];
		UniqueIDWrapper id = this->wrapper->GetUniqueID();
		MMRWrapper mmr = this->wrapper->GetMMRWrapper();
		RatingRequestResult result = stats->rating->RequestUpdate(this->wrapper.get(), playlist);
		if (result == RatingRequestResult::NOT_SYNCED)
		{
			return RatingUpdateResult::NOT_SYNCED;
		}
		else if (result == RatingRequestResult::SUCCESS_WIN)
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
		Log("StatTracker::Update(const RankedPlaylist) - Update finished");
		return RatingUpdateResult::SUCCESS;
	}
}
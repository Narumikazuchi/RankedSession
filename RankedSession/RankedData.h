#pragma once

#include <string>
#include "bakkesmod/plugin/bakkesmodplugin.h"
#include "Util.h"

namespace RankedSession
{
	enum RankedPlaylist
	{
		RANKED_DUEL = 10,
		RANKED_DOUBLE = 11,
		RANKED_STANDARD = 13,
		RANKED_HOOPS = 27,
		RANKED_RUMBLE = 28,
		RANKED_DROPSHOT = 29,
		RANKED_SNOWDAY = 30,
		TOURNAMENT = 34
	};

	enum Rank
	{
		SupersonicLegend = 22,
		GrandChamp3 = 21,
		GrandChamp2 = 20,
		GrandChamp1 = 19,
		Champ3 = 18,
		Champ2 = 17,
		Champ1 = 16,
		Diamond3 = 15,
		Diamond2 = 14,
		Diamond1 = 13,
		Platinum3 = 12,
		Platinum2 = 11,
		Platinum1 = 10,
		Gold3 = 9,
		Gold2 = 8,
		Gold1 = 7,
		Silver3 = 6,
		Silver2 = 5,
		Silver1 = 4,
		Bronze3 = 3,
		Bronze2 = 2,
		Bronze1 = 1,
		Unranked = 0
	};

	// The color and name information for every tier.
	struct RankInfo
	{
	public:
		LinearColor color;
		std::string name;
		std::shared_ptr<ImageWrapper> image;
	};

	std::string GetPlaylistName(const RankedPlaylist playlist);
	std::string GetRankName(const Rank tier, const int division);
	bool IsPlaylistValid(const RankedPlaylist playlist);

	extern std::map<Rank, RankInfo> RankInfoDatabase;
	extern std::vector<RankedPlaylist> AvailablePlaylists;
}


inline std::string RankedSession::GetPlaylistName(const RankedSession::RankedPlaylist playlist)
{
	switch (playlist)
	{
	case RankedPlaylist::RANKED_DUEL:
		return "1v1 Duels";
	case RankedPlaylist::RANKED_DOUBLE:
		return "2v2 Doubles";
	case RankedPlaylist::RANKED_STANDARD:
		return "3v3 Standard";
	case RankedPlaylist::RANKED_HOOPS:
		return "2v2 Hoops";
	case RankedPlaylist::RANKED_RUMBLE:
		return "3v3 Rumble";
	case RankedPlaylist::RANKED_DROPSHOT:
		return "3v3 Dropshot";
	case RankedPlaylist::RANKED_SNOWDAY:
		return "3v3 Snowday";
	case RankedPlaylist::TOURNAMENT:
		return "3v3 Tournaments";
	default:
		return "Unknown";
	}
}

inline std::string RankedSession::GetRankName(const RankedSession::Rank tier, const int division)
{
	if (tier < 0 ||
		tier > 22)
	{
		return "ERROR";
	}

	std::string rankName = RankedSession::RankInfoDatabase[tier].name;
	if (tier != RankedSession::Rank::Unranked &&
		tier != RankedSession::Rank::SupersonicLegend)
	{
		rankName += " Div " + std::to_string(division + 1);
	}

	return rankName;
}

inline bool RankedSession::IsPlaylistValid(const RankedSession::RankedPlaylist playlist)
{
	return RankedSession::VectorHasItem(&RankedSession::AvailablePlaylists, playlist);
}
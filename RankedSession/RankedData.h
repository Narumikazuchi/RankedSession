#pragma once

#include <string>
#include <map>
#include "bakkesmod/plugin/bakkesmodplugin.h"

namespace RankedSession
{
	enum class RankedPlaylist
	{
		ONES = 10,
		TWOS = 11,
		THREES = 13,
		HOOPS = 27,
		RUMBLE = 28,
		DROPSHOT = 29,
		SNOWDAY = 30,
		TOURNAMENT = 34
	};

	enum class Rank
	{
		SuperSonicLegend = 22,
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
	};

	std::string GetPlaylistName(const RankedSession::RankedPlaylist playlist);

	extern std::map<Rank, RankInfo> RankInfoDatabase;
	extern std::vector<RankedPlaylist> AvailablePlaylists;
}


inline std::string RankedSession::GetPlaylistName(const RankedSession::RankedPlaylist playlist)
{
	switch (playlist)
	{
	case RankedPlaylist::ONES:
		return "1v1 Duels";
	case RankedPlaylist::TWOS:
		return "2v2 Doubles";
	case RankedPlaylist::THREES:
		return "3v3 Standard";
	case RankedPlaylist::HOOPS:
		return "2v2 Hoops";
	case RankedPlaylist::RUMBLE:
		return "3v3 Rumble";
	case RankedPlaylist::DROPSHOT:
		return "3v3 Dropshot";
	case RankedPlaylist::SNOWDAY:
		return "3v3 Snowday";
	case RankedPlaylist::TOURNAMENT:
		return "3v3 Tournaments";
	default:
		return "Unknown";
	}
}
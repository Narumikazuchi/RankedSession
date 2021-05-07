#include "RankedData.h"

namespace RankedSession
{
    std::map<Rank, RankInfo> RankInfoDatabase
    {
        { Rank::Unranked         ,{{ 133, 133, 133 }, "Unranked", nullptr}},
        { Rank::Bronze1          ,{{ 227, 151, 68  }, "Bronze 1", nullptr}},
        { Rank::Bronze2          ,{{ 227, 151, 68  }, "Bronze 2", nullptr}},
        { Rank::Bronze3          ,{{ 227, 151, 68  }, "Bronze 3", nullptr}},
        { Rank::Silver1          ,{{ 133, 133, 133 }, "Silver 1", nullptr}},
        { Rank::Silver2          ,{{ 133, 133, 133 }, "Silver 2", nullptr}},
        { Rank::Silver3          ,{{ 133, 133, 133 }, "Silver 3", nullptr}},
        { Rank::Gold1            ,{{ 202, 149, 31  }, "Gold 1", nullptr}},
        { Rank::Gold2            ,{{ 202, 149, 31  }, "Gold 2", nullptr}},
        { Rank::Gold3            ,{{ 202, 149, 31  }, "Gold 3", nullptr}},
        { Rank::Platinum1        ,{{ 50, 204, 252  }, "Platinum 1", nullptr}},
        { Rank::Platinum2        ,{{ 50, 204, 252  }, "Platinum 2", nullptr}},
        { Rank::Platinum3        ,{{ 50, 204, 252  }, "Platinum 3", nullptr}},
        { Rank::Diamond1         ,{{ 2, 188, 255   }, "Diamond 1", nullptr}},
        { Rank::Diamond2         ,{{ 2, 188, 255   }, "Diamond 2", nullptr}},
        { Rank::Diamond3         ,{{ 2, 188, 255   }, "Diamond 3", nullptr}},
        { Rank::Champ1           ,{{ 202, 137, 255 }, "Champion 1", nullptr}},
        { Rank::Champ2           ,{{ 202, 137, 255 }, "Champion 2", nullptr}},
        { Rank::Champ3           ,{{ 202, 137, 255 }, "Champion 3", nullptr}},
        { Rank::GrandChamp1      ,{{ 244, 56, 236  }, "Grand Champion 1", nullptr}},
        { Rank::GrandChamp2      ,{{ 244, 56, 236  }, "Grand Champion 2", nullptr}},
        { Rank::GrandChamp3      ,{{ 244, 56, 236  }, "Grand Champion 3", nullptr}},
        { Rank::SupersonicLegend ,{{ 224, 224, 224 }, "Supersonic Legend", nullptr}}
    };

    std::vector<RankedPlaylist> AvailablePlaylists
    {
        RankedPlaylist::RANKED_DUEL,
        RankedPlaylist::RANKED_DOUBLE,
        RankedPlaylist::RANKED_STANDARD,
        RankedPlaylist::RANKED_HOOPS,
        RankedPlaylist::RANKED_RUMBLE,
        RankedPlaylist::RANKED_DROPSHOT,
        RankedPlaylist::RANKED_SNOWDAY,
        RankedPlaylist::TOURNAMENT
    };
}
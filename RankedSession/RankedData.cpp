#include "RankedData.h"

namespace RankedSession
{
    std::map<Rank, RankInfo> RankInfoDatabase
    {
        { Rank::Unranked         ,{{ 133, 133, 133 }, "Unranked"}},
        { Rank::Bronze1          ,{{ 227, 151, 68  }, "Bronze 1"}},
        { Rank::Bronze2          ,{{ 227, 151, 68  }, "Bronze 2"}},
        { Rank::Bronze3          ,{{ 227, 151, 68  }, "Bronze 3"}},
        { Rank::Silver1          ,{{ 133, 133, 133 }, "Silver 1"}},
        { Rank::Silver2          ,{{ 133, 133, 133 }, "Silver 2"}},
        { Rank::Silver3          ,{{ 133, 133, 133 }, "Silver 3"}},
        { Rank::Gold1            ,{{ 202, 149, 31  }, "Gold 1"}},
        { Rank::Gold2            ,{{ 202, 149, 31  }, "Gold 2"}},
        { Rank::Gold3            ,{{ 202, 149, 31  }, "Gold 3"}},
        { Rank::Platinum1        ,{{ 50, 204, 252  }, "Platinum 1"}},
        { Rank::Platinum2        ,{{ 50, 204, 252  }, "Platinum 2"}},
        { Rank::Platinum3        ,{{ 50, 204, 252  }, "Platinum 3"}},
        { Rank::Diamond1         ,{{ 2, 188, 255   }, "Diamond 1"}},
        { Rank::Diamond2         ,{{ 2, 188, 255   }, "Diamond 2"}},
        { Rank::Diamond3         ,{{ 2, 188, 255   }, "Diamond 3"}},
        { Rank::Champ1           ,{{ 202, 137, 255 }, "Champion 1"}},
        { Rank::Champ2           ,{{ 202, 137, 255 }, "Champion 2"}},
        { Rank::Champ3           ,{{ 202, 137, 255 }, "Champion 3"}},
        { Rank::GrandChamp1      ,{{ 244, 56, 236  }, "Grand Champion 1"}},
        { Rank::GrandChamp2      ,{{ 244, 56, 236  }, "Grand Champion 2"}},
        { Rank::GrandChamp3      ,{{ 244, 56, 236  }, "Grand Champion 3"}},
        { Rank::SuperSonicLegend ,{{ 224, 224, 224 }, "Super Sonic Legend"}}
    };

    std::vector<RankedPlaylist> AvailablePlaylists
    {
        RankedPlaylist::ONES,
        RankedPlaylist::TWOS,
        RankedPlaylist::THREES,
        RankedPlaylist::HOOPS,
        RankedPlaylist::RUMBLE,
        RankedPlaylist::DROPSHOT,
        RankedPlaylist::SNOWDAY,
        RankedPlaylist::TOURNAMENT
    };
}
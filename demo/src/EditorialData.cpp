#include "EditorialData.h"

std::shared_ptr<GameInfo> GameInfo::parse(const Json::Value& game)
{
    std::shared_ptr<GameInfo> gameInfo = std::make_shared<GameInfo>();

    // we currently only want content which provides title, subheading and image urls
    Json::Value mlb = game["content"]["editorial"]["recap"]["mlb"];
    Json::Value hl = mlb["headline"];
    Json::Value subhead = mlb["subhead"];
    Json::Value date = mlb["date"];
    Json::Value id = mlb["id"];
    Json::Value blurb = mlb["blurb"];

    gameInfo->_headline = hl.asString();
    gameInfo->_subHead = subhead.asString();
    gameInfo->_date = date.asString();
    gameInfo->_id = id.asString();
    gameInfo->_blurb = blurb.asString();

    //static const char* defaultSize = "1920x1080";
    static const char* defaultSize = "124x70";
    Json::Value size = mlb["photo"]["cuts"][defaultSize];
    Json::Value src = size["src"];
    Json::Value w = size["width"];
    Json::Value h = size["height"];
    gameInfo->_imgURL = src.asString();
    gameInfo->_imgWidth = w.asUInt();
    gameInfo->_imgHeight = h.asUInt();

    return gameInfo;
}

std::shared_ptr<EditorialData> EditorialData::parse(const Json::Value& root)
{
    std::shared_ptr<EditorialData> data = std::make_shared<EditorialData>();
    data->_copyright = root["copyright"].asString();
    
    // verify number of games entries matches the parsed games
    const unsigned int totalGames = root["totalGames"].asUInt();
    data->_games.reserve(totalGames);

    Json::Value dates = root["dates"];
    for(Json::Value::iterator date = dates.begin(); date != dates.end(); ++date)
    {
        Json::Value key = date.key();
        //Json::Value value = (*it);
        Json::Value games = (*date)["games"];
        int gameNum = 0;
        for(Json::Value::iterator game = games.begin(); game != games.end(); ++game)
        {
            gameNum++;
            //printf("GameNum: %d\n", gameNum);
            std::shared_ptr<GameInfo> g = GameInfo::parse(*game);
            data->_games.push_back(g);
            #if 0
            printf("Headline: %s subhead: %s\n", g->_headline.c_str(), g->_subHead.c_str());
            printf("ImgURL: %s, width: %u height: %u\n", g->_imgURL.c_str(), g->_imgWidth, g->_imgHeight);
            #endif
        }
    }

    #if 0
    // do the parsed games match our expected values?
    if(data->_games.size() == totalGames)
    {
        printf("Total games %d match expected games: %d\n", totalGames, static_cast<int>(data->_games.size()));
    }else{
        printf("ERROR: Total games %d NOT NOT MATCH expected games: %d\n", totalGames, static_cast<int>(data->_games.size()));
    }
    #endif

    return data;
}

#include "Demo.h"

#include "oxygine/json/json.h"

#include <memory> // for shared_ptr

static const bool DEBUG_RESOURCES_ONLY = false;

static const std::string DEBUG_JSON_URL="https://jsonplaceholder.typicode.com/todos/1";
// this will produce a response like:
/*
{
  "userId": 1,
  "id": 1,
  "title": "delectus aut autem",
  "completed": false
}
*/
static const std::string BACKGROUND_URL="http://mlb.mlb.com/mlb/images/devices/ballpark/1920x1080/1.jpg";

extern Resources gameResources;

struct GameInfo
{
    std::string _headline;
    std::string _subHead;
    std::string _imgURL;
    unsigned int _imgWidth = 0;
    unsigned int _imgHeight = 0;
    std::string _date;
    std::string _id;

    static std::shared_ptr<GameInfo> parse(const Json::Value& game)
    {
        std::shared_ptr<GameInfo> gameInfo = std::make_shared<GameInfo>();
        #if 0
        gameInfo->_title = "Title";
        gameInfo->_desc = "Desc";
        gameInfo->_imgURL = "http://idontknow.com";
        #else
        //"season" : "2018",
        //"gameDate" : "2018-06-10T17:05:00Z",
        // "content" : {
        // "link" : "/api/v1/game/530376/content",
        // "editorial" : {
        //   "recap" : {
        //     "home" : {
        //       "type" : "article",
        //       "state" : "A",
        //       "date" : "2018-06-10T14:26:31-0400",
        //       "id" : "280642806",
        //       "headline" : "Lopez stifles Red Sox as White Sox take 2 of 3",
        //       "subhead" : "Chicago now 6-4 in June; 'Guys just are enjoying the game'",

        // we currently only want content which provides title, subheading and image urls
        Json::Value content = game["content"];
        Json::Value editorial = content["editorial"];
        Json::Value recap = editorial["recap"];
        Json::Value home = recap["home"];
        Json::Value hl = home["headline"];
        Json::Value subhead = home["subhead"];

        Json::Value date = home["date"];
        Json::Value id = home["id"];

        gameInfo->_headline = hl.asString();
        gameInfo->_subHead = subhead.asString();
        gameInfo->_date = date.asString();
        gameInfo->_id = id.asString();

        // "photo" : {
        //         "title" : "Lopez stifles Red Sox as White Sox take 2 of 3",
        //         "altText" : "",
        //         "cuts" : {
        //           "1920x1080" : {
        //             "aspectRatio" : "16:9",
        //             "width" : 1920,
        //             "height" : 1080,
        //             "src" : "https://securea.mlb.com/assets/images/7/9/6/280664796/cuts/1920x1080/cut.jpg",
        Json::Value photo = home["photo"];
        Json::Value cuts = photo["cuts"];
        //static const char* defaultSize = "1920x1080";
        static const char* defaultSize = "124x70";
        
        Json::Value size = cuts[defaultSize];
        Json::Value src = size["src"];
        Json::Value w = size["width"];
        Json::Value h = size["height"];
        gameInfo->_imgURL = src.asString();
        gameInfo->_imgWidth = w.asUInt();
        gameInfo->_imgHeight = h.asUInt();
        #endif

        return gameInfo;
    }
};

struct EditorialData
{
    std::string _copyright;
    std::vector< std::shared_ptr<GameInfo> > _games;

    static std::shared_ptr<EditorialData> parse(const Json::Value& root)
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
                printf("GameNum: %d\n", gameNum);
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
};


void Demo::onAdded2Stage()
{
    // async set background image
    setBackground();

    fetchEditorialData();
}

void Demo::fetchEditorialData()
{
    // TODO: url for today's date or specific date
    const std::string URL = "http://statsapi.mlb.com/api/v1/schedule\?hydrate=game(content(editorial(recap))),decisions&date=2018-06-10&sportId=1";

    // do an async request for json data
    ox::spHttpRequestTask task = ox::HttpRequestTask::create();
    if(DEBUG_RESOURCES_ONLY)
        task->setUrl(DEBUG_JSON_URL);
    else
        task->setUrl(URL);
    
    // Initiate the http request and handle async response
    printf("initiating json request...\n");
    spDemo self = this;
    task->addEventListener(HttpRequestTask::COMPLETE, [self](Event* event){

        ox::HttpRequestTask* task = safeCast<ox::HttpRequestTask*>(event->currentTarget.get());
        const std::vector<unsigned char> &response = task->getResponse();
        printf("Size of response vector: %d\n", static_cast<int>(response.size()));
        std::string resp;
        resp.assign(response.begin(), response.end());
        printf("Size of response string: %d\n", static_cast<int>(resp.size()));

        #if 0
        ox::logs::messageln("server response: %s", resp.c_str());
        #elif 0
        // parse the json and get info we want out
        Json::Value root;
        Json::Reader reader;
        bool parsingSuccessful = reader.parse( resp, root );
        if ( !parsingSuccessful )
        {
            printf("Error parsing json string...\n");
        }

        const Json::Value mynames = root["title"];

        printf("Json parsed title: %s\n", mynames.asString().c_str());

        #else
        self->parseEditorialData(resp);
        #endif
    });

    task->run();
}

void Demo::parseEditorialData(const std::string& data)
{
    // basic layout of json response should be like:
    // {
    //     "copyright" : "Copyright 2019 MLB Advanced Media, L.P.  Use of any content on this page acknowledges agreement to the terms posted here http://gdx.mlb.com/components/copyright.txt",
    //     "totalItems" : 15,
    //     "totalEvents" : 0,
    //     "totalGames" : 15,
    //     "totalGamesInProgress" : 0,
    //     "dates" : [ {
    //         "date" : "2018-06-10",
    //         "totalItems" : 15,
    //         "totalEvents" : 0,
    //         "totalGames" : 15,
    //         "totalGamesInProgress" : 0,
    //         ...
    //         ...
    //         "games" : [ {
    //             "content" : {
    //             "link" : "/api/v1/game/530376/content",
    //             "editorial" : {
    //             "recap" : {
    //                 "home" : {
    //                 "type" : "article",

    printf("Parsing editorial data...\n");

    // parse the json and get info we want out
    Json::Value root;
    Json::Reader reader;
    bool parsingSuccessful = reader.parse( data, root );
    if ( !parsingSuccessful )
    {
        printf("ERROR parsing json string...\n");
        return;
    }

    printf("Parsing editorial data complete...\n");

    std::shared_ptr<EditorialData> editorialData = EditorialData::parse(root);

    #if 1
    // commonsense checking of editorial data returned
    printf("Editorial data copyright: %s\n", editorialData->_copyright.c_str());
    #endif

}


void Demo::setBackground()
{
    HttpRequestTask::init();

    // actual async load of bg image
    spWebImage sp = new WebImage;
    if(DEBUG_RESOURCES_ONLY)
        sp->load("http://oxygine.org/test/logo.png");
    else
        sp->load(BACKGROUND_URL);
    
    sp->setTouchEnabled(false);
    sp->attachTo(getStage());
    const float w = getStage()->getWidth();
    const float h = getStage()->getHeight();
    sp->setSize(w,h);
    sp->setX(0);
    sp->setY(0);
}

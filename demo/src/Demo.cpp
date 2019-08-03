#include "Demo.h"

#include "oxygine/json/json.h"

#include <memory> // for shared_ptr
#include <ctime>
#include <locale>

static const bool DEBUG_RESOURCES_ONLY = false;
static const unsigned int FEED_LOC_Y = 200;
static const unsigned int FEED_BORDER = 20;
unsigned int DEFAULT_IMG_WIDTH = 124;
unsigned int DEFAULT_IMG_HEIGHT = 70;

static const std::string BACKGROUND_URL="http://mlb.mlb.com/mlb/images/devices/ballpark/1920x1080/1.jpg";

extern Resources gameResources;

void Demo::onAdded2Stage()
{

    // the actual menu we're implementing
    float screenWidth = getStage()->getWidth();
    _menu = new EditorialMenu();
    _menu->setY(FEED_LOC_Y);
    _menu->setX(screenWidth/2.0f);
    _menu->setHeight( DEFAULT_IMG_HEIGHT + 2 * FEED_BORDER);
    _menu->setWidth(screenWidth);
    _menu->setPriority(100);
    getStage()->addChild(_menu);

    #if 1
    // setup UI
    spDemo self = this;
	ox::core::getDispatcher()->addEventListener(ox::core::EVENT_SYSTEM,[self](Event* ev)
    {
        SDL_Event *event = (SDL_Event*)ev->userData;

        if (event->type != SDL_KEYDOWN)
            return;

        //all key codes could be found in "SDL_keyboard.h" from SDL
        switch (event->key.keysym.sym)
        {
            case SDLK_LEFT:			
                logs::messageln("LEFT KEYDOWN\n");
                self->_menu->moveLeft();
            break;
            case SDLK_RIGHT:			
                logs::messageln("RIGHT KEYDOWN\n");
                self->_menu->moveRight();
            break;
            case SDLK_RETURN:
                logs::messageln("RETURN KEYDOWN\n");
                self->_menu->showHideDetails();
            break;
        }
    });
    #endif

    // async set background image
    setBackground();

    fetchEditorialData();
}

void Demo::doUpdate(const UpdateState& us)
{
    // once we have cached editorial data we can start displaying
    static bool doInit = true;
    if(doInit && _parsedEditorialData)
    {
        doInit = false;

        for(auto g:_parsedEditorialData->_games)
        {
            _menu->addDisplay(g);
        }
    }

    return Actor::doUpdate(us);
}

void Demo::fetchEditorialData()
{
    // TODO: manage what day (i.e. N days ago)
    const std::string URL = Demo::generateJsonURL(2);

    // do an async request for json data
    ox::spHttpRequestTask task = ox::HttpRequestTask::create();
    task->setUrl(URL);
    
    // Initiate the http request and handle async response
    logs::messageln("initiating json request...\n");
    spDemo self = this;
    task->addEventListener(HttpRequestTask::COMPLETE, [self](Event* event){

        ox::HttpRequestTask* task = safeCast<ox::HttpRequestTask*>(event->currentTarget.get());
        const std::vector<unsigned char> &response = task->getResponse();
        std::string resp;
        resp.assign(response.begin(), response.end());

        self->parseEditorialData(resp);
    });

    task->addEventListener(HttpRequestTask::ERROR, [URL](Event* event){
        logs::error("ERROR: unable to fetch url: %s", URL.c_str());
    });

    task->run();
}

void Demo::parseEditorialData(const std::string& data)
{
    // TODO: measure the time to parse here as it's probably expensive

    Json::Value root;
    Json::Reader reader;
    bool parsingSuccessful = reader.parse( data, root );
    if ( !parsingSuccessful )
    {
        logs::error("ERROR parsing json string...\n");
        return;
    }

    _parsedEditorialData = EditorialData::parse(root);

    #if 1
    // commonsense checking of editorial data returned
    logs::messageln("Editorial data copyright: %s\n", _parsedEditorialData->_copyright.c_str());
    #endif

    root.clear();
}


void Demo::setBackground()
{
    // actual async load of bg image
    // TODO: add to scene graph early, but set image late
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
    sp->setPriority(-100);
}

std::string Demo::generateJsonURL(const unsigned int daysAgo /* = 0*/)
{
    #if 0
    // Static URL with known behavior for debugging
    std::string URL = "http://statsapi.mlb.com/api/v1/schedule\?hydrate=game(content(editorial(recap))),decisions&date=2018-06-10&sportId=1";
    #else
    std::string URL = "http://statsapi.mlb.com/api/v1/schedule\?hydrate=game(content(editorial(recap))),decisions&sportId=1";
    
    // after: https://en.cppreference.com/w/cpp/chrono/c/strftime
    #if 0
    // TODO: we may need locale support?
    std::locale::global(std::locale("ja_JP.utf8"));
    #endif
    std::time_t now = std::time(nullptr);
    std::time_t t = now - daysAgo * (24*60*60);

    // TODO: maybe  std::wcsftime is necessary? Maybe not. URL is only ascii
    char mbstr[64];
    // strftime returns characters written or 0 if character buffer exceeded
    std::strftime(mbstr, sizeof(mbstr), "&date=%Y-%m-%d", std::localtime(&t));
    URL += mbstr;

    #endif

    return URL;
}

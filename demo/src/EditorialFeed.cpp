#include "EditorialFeed.h"

#include "oxygine/json/json.h"

#include <memory> // for shared_ptr
#include <ctime>
#include <locale>

static const unsigned int FEED_LOC_Y = 200;
static const unsigned int FEED_BORDER = 20;
unsigned int DEFAULT_IMG_WIDTH = 124;
unsigned int DEFAULT_IMG_HEIGHT = 70;

//#define USE_COMPILE_TIME_BG

static const std::string DEFAULT_FONT = "freeSans";

extern Resources gameResources;

void EditorialFeed::onAdded2Stage()
{
    // General setup and start initialization

    float screenWidth = getStage()->getWidth();
    _menu = new EditorialMenu();
    _menu->setY(FEED_LOC_Y);
    _menu->setX(screenWidth/2.0f);
    _menu->setHeight( DEFAULT_IMG_HEIGHT + 2 * FEED_BORDER);
    _menu->setWidth(screenWidth);
    _menu->setPriority(100);
    addChild(_menu);

    // setup UI
    spEditorialFeed self = this;
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
            case SDLK_UP:			
                logs::messageln("UP KEYDOWN\n");
                self->showPreviousDay();
            break;
            case SDLK_DOWN:			
                logs::messageln("DOWN KEYDOWN\n");
                self->showNextDay();
            break;
            case SDLK_RETURN:
                logs::messageln("RETURN KEYDOWN\n");
                self->_menu->showHideDetails();
            break;
        }
    });

    // async set background image
    setBackground();

    // async build menu for current day
    fetchEditorialData(_daysAgo);

    // put some simple instructions at the bottom of the page_instructions;
    _instructions = new TextField();
    addChild(_instructions);
    _instructions->setPosition(0, getStage()->getHeight() - 40);
    _instructions->setSize(getStage()->getWidth(), 40);
    _instructions->setText("Use Left/Right arrows to browse Articles. Up/Down arrows for day. Return key for details.");
    _instructions->setHAlign(TextStyle::HALIGN_MIDDLE);
    _instructions->setColor(Color::White);
    _instructions->setFontSize(24);
    _instructions->setFont(gameResources.getResFont(DEFAULT_FONT));
}

void EditorialFeed::doUpdate(const UpdateState& us)
{
    // "dirty" means we have new json data and have to initialize a new Menu display
    if(_dirty)
    {
        _menu->clear();
        auto data = _parsedEditorialData[_daysAgo];

        if(!_date)
        {
            _date = new TextField();
            addChild(_date);
            _date->setPosition(0,0);
            _date->setSize(getStage()->getWidth(), 40);
            _date->setHAlign(TextStyle::HALIGN_MIDDLE);
            _date->setColor(Color::White);
            _date->setFontSize(24);
            _date->setFont(gameResources.getResFont(DEFAULT_FONT));
        }
        _date->setText(data->_date);


        for(auto g:data->_games)
        {
            _menu->addDisplay(g);
        }
        _dirty = false;
    }

    return Actor::doUpdate(us);
}

void EditorialFeed::fetchEditorialData(const unsigned int daysAgo)
{
    // No need to fetch data if we already have it
    if(_parsedEditorialData.find(daysAgo) != _parsedEditorialData.end())
    {
        _daysAgo = daysAgo;
        _dirty = true;
        return;
    }

    const std::string URL = EditorialFeed::generateJsonURL(daysAgo);

    // do an async request for json data
    ox::spHttpRequestTask task = ox::HttpRequestTask::create();
    task->setUrl(URL);
    
    // Initiate the http request and handle async response
    logs::messageln("initiating json request...\n");
    spEditorialFeed self = this;
    task->addEventListener(HttpRequestTask::COMPLETE, [daysAgo, self](Event* event){

        ox::HttpRequestTask* task = safeCast<ox::HttpRequestTask*>(event->currentTarget.get());
        const std::vector<unsigned char> &response = task->getResponse();
        self->_outstandingJSONRequest = false;
        self->_daysAgo = daysAgo;
        self->parseEditorialData(response);
    });

    task->addEventListener(HttpRequestTask::ERROR, [self, URL](Event* event){
        logs::error("ERROR: unable to fetch url: %s", URL.c_str());
        self->_outstandingJSONRequest = false;
    });

    task->run();
}

void EditorialFeed::parseEditorialData(const std::vector<unsigned char>& data)
{
    // TODO: measure the time to parse here as it's probably expensive

    Json::Value root;
    Json::Reader reader;
    size_t sz = data.size();
    const char* start = reinterpret_cast<const char*>(&(data[0]));
    const char* end = reinterpret_cast<const char*>(&(data[sz]));
    bool parsingSuccessful = reader.parse(start, end, root, false);
    if ( !parsingSuccessful )
    {
        logs::error("ERROR parsing json string: %s\n", reader.getFormatedErrorMessages().c_str());
        return;
    }

    // This cache currently only grows.
    // In practice we'd need to reduce it above some size limit
    _parsedEditorialData[_daysAgo] = EditorialData::parse(root);
    _dirty = true;

}


void EditorialFeed::setBackground()
{
    #if defined(USE_COMPILE_TIME_BG)
    // Use a pre-packaged background image so it can be hardware compressed.
    spSprite sp = new Sprite();
    sp->setResAnim(gameResources.getResAnim("bg"));
    #else
    static const std::string BACKGROUND_URL="http://mlb.mlb.com/mlb/images/devices/ballpark/1920x1080/1.jpg";
    // Background is currently held as R8G8B8A8
    // As it's 1920x1080 background image goes from 1.6M compressed
    // to (1920x1080x4 bytes) 8.3MB!!!
    // TODO: resize the image to our current screen (i.e. if the screen is smaller)?
    // TODO: Use a more compact Image format?
    // TODO: Use a HW compressed texture?
    spWebImage sp = new WebImage;
    sp->load(BACKGROUND_URL);
    #endif
    
    sp->setTouchEnabled(false);
    sp->attachTo(getStage());
    const float w = getStage()->getWidth();
    const float h = getStage()->getHeight();
    sp->setSize(w,h);
    sp->setX(0);
    sp->setY(0);
    sp->setPriority(-100);
}

std::string EditorialFeed::generateJsonURL(const unsigned int daysAgo /* = 0*/)
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

    void EditorialFeed::showPreviousDay()
    {
        if(_outstandingJSONRequest) return;

        fetchEditorialData(_daysAgo + 1);
    }

    void EditorialFeed::showNextDay()
    {
        if(_outstandingJSONRequest || _daysAgo == 0) return;

        fetchEditorialData(_daysAgo - 1);
    }

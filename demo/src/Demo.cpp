#include "Demo.h"
#include "EditorialDisplay.h"

#include "oxygine/json/json.h"

#include <memory> // for shared_ptr

static const bool DEBUG_RESOURCES_ONLY = false;
static const unsigned int FEED_LOC_Y = 200;
static const unsigned int FEED_BORDER = 20;
unsigned int DEFAULT_IMG_WIDTH = 124;
unsigned int DEFAULT_IMG_HEIGHT = 70;

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

void Demo::onAdded2Stage()
{
    #if 1
    // debugging bg behind images
    _root = new ColorRectSprite();
    _root->setColor(Color::Yellow);
    //_root->setAlpha(0.5);
    #else
    _root = new Actor();
    #endif
    _root->setY(FEED_LOC_Y);
    //_root->setX(getStage()->getWidth() / 2.0);
    _root->setX(0);
    _root->setHeight( DEFAULT_IMG_HEIGHT + 2 * FEED_BORDER);
    _root->setWidth(getStage()->getWidth());
    //_root->setAnchor(0.5, 0.5);
    _root->setPriority(100);
    addChild(_root);

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

        //const unsigned int numGames = static_cast<unsigned int>(_parsedEditorialData->_games.size());
        unsigned int editorialXPos = 0;
        const unsigned int editoralYPos = FEED_BORDER;
        for(auto g:_parsedEditorialData->_games)
        {
            spEditorialDisplay d = new EditorialDisplay(g);
            d->setPosition(editorialXPos, editoralYPos);
            _root->addChild(d);
            _assetQueue.push_back(d);
            editorialXPos += DEFAULT_IMG_WIDTH;
        }
    }

    // If we have things to fetch in the queue, do so
    if(_assetQueue.size())
    {
        auto asset = _assetQueue.front();
        if(asset->inflated())
        {
            // done. kick off the next one next update
            _assetQueue.pop_front();
        }
        else if(!asset->isInflating())
        {
            // kick off fetching new assets
            asset->inflate();
        }
    }

    return Actor::doUpdate(us);
}

void Demo::fetchEditorialData()
{
    // TODO: url for today's date or specific date
    const std::string URL = "http://statsapi.mlb.com/api/v1/schedule\?hydrate=game(content(editorial(recap))),decisions&date=2018-06-10&sportId=1";

    // do an async request for json data
    ox::spHttpRequestTask task = ox::HttpRequestTask::create();
    task->setUrl(URL);
    
    // Initiate the http request and handle async response
    printf("initiating json request...\n");
    spDemo self = this;
    task->addEventListener(HttpRequestTask::COMPLETE, [self](Event* event){

        ox::HttpRequestTask* task = safeCast<ox::HttpRequestTask*>(event->currentTarget.get());
        const std::vector<unsigned char> &response = task->getResponse();
        std::string resp;
        resp.assign(response.begin(), response.end());

        self->parseEditorialData(resp);
    });

    task->addEventListener(HttpRequestTask::ERROR, [URL](Event* event){
        printf("ERROR: unable to fetch url: %s", URL.c_str());
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
        printf("ERROR parsing json string...\n");
        return;
    }

    _parsedEditorialData = EditorialData::parse(root);

    #if 1
    // commonsense checking of editorial data returned
    printf("Editorial data copyright: %s\n", _parsedEditorialData->_copyright.c_str());
    #endif
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

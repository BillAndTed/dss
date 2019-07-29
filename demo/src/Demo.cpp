#include "Demo.h"

#include "oxygine/json/json.h"

static const bool DEBUG_RESOURCES_ONLY = false;

static const std::string DEFAULT_JSON_URL="https://jsonplaceholder.typicode.com/todos/1";
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
    // async set background image
    setBackground();

    #if 1
    // do an async request for json data
    ox::spHttpRequestTask task = ox::HttpRequestTask::create();
    task->setUrl(DEFAULT_JSON_URL);
    task->addEventListener(HttpRequestTask::COMPLETE, [](Event* event){
            ox::HttpRequestTask* task = safeCast<ox::HttpRequestTask*>(event->currentTarget.get());
        
        const std::vector<unsigned char> &response = task->getResponse();

        //convert it to string and print
        std::string resp;
        resp.assign(response.begin(), response.end());

        #if 0
        ox::logs::messageln("server response: %s", resp.c_str());
        #else
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

        #endif
    });

    task->run();
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

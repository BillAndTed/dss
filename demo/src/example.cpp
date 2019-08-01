#include "oxygine-framework.h"
#include <functional>

#include "Demo.h"

using namespace oxygine;

//it is our resources
//in real project you would have more than one Resources declarations.
//It is important on mobile devices with limited memory and you would load/unload them
Resources gameResources;

void example_preinit() {}

//called from main.cpp
void example_init()
{
    // I'd prefer a compile time solution to turn off logs
    // but this is currently all the framework supports
    #ifndef OX_DEBUG
    logs::disable();
    #endif

    ox::HttpRequestTask::init();

    //load xml file with resources definition
    gameResources.loadXML("res.xml");

    #if 1
    spDemo demo = new Demo;

    //and add it to Stage as child
    getStage()->addChild(demo);
    #endif
}


//called each frame from main.cpp
void example_update()
{
}

//called each frame from main.cpp
void example_destroy()
{
    ox::HttpRequestTask::release();

    //free previously loaded resources
    gameResources.free();
}

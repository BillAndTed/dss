#pragma once

#include "oxygine-framework.h"
#include "EditorialData.h"

using namespace oxygine;

DECLARE_SMART(EditorialDisplay, spEditorialDisplay)
class EditorialDisplay: public Actor
{
private:
    // colored bg for debugging
    #if 1
    spColorRectSprite _root;
    #else
    spActor _root;
    #endif

    std::shared_ptr<GameInfo> _gameInfo;
    bool _inflating = false;
    bool _inflated = false;

    spWebImage _image;

public:
    EditorialDisplay(std::shared_ptr<GameInfo> gameInfo);

    /*
        Has this widget fetched it async assets yet?
     */
    bool inflated() const {return _inflated;}

    /* currently fetching assets */
    bool isInflating() const {return _inflating;}
    /*
        Initiate async fetch of assets
     */
    void inflate();
    /*
        Release resources if this is offscreen/not needed
     */
    void deflate();

};
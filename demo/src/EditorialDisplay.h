#pragma once

#include "oxygine-framework.h"
#include "EditorialData.h"

using namespace oxygine;

DECLARE_SMART(EditorialDisplay, spEditorialDisplay)
class EditorialDisplay: public Actor
{
private:
    std::shared_ptr<GameInfo> _gameInfo;
    bool _highlighted = false;

    spWebImage _image;
    spTextField _headline;
    spTextField _subText;
    bool _inflated = false;

public:
    std::shared_ptr<GameInfo> getGameInfo() const { return _gameInfo; }
    spWebImage getImage() const { return _image; };

public:
    EditorialDisplay(std::shared_ptr<GameInfo> gameInfo);
    /*
    Have we requested assets off the server?
     */
    bool inflated() const { return _inflated; };

    /*
        Initiate async fetch of assets
     */
    void inflate();
    /*
        Release resources if this is offscreen/not needed
     */
    void deflate();

    /*
        Are we highlighed?
     */
    bool isHighlighted() const {return _highlighted;}
    void setHighlight(const bool h);

    /*
    helper for position tweens
     */
    void moveTo(const float x, const float y, const bool animate = true);

};
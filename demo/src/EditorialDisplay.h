#pragma once

#include "oxygine-framework.h"
#include "EditorialData.h"

using namespace oxygine;

DECLARE_SMART(EditorialDisplay, spEditorialDisplay)
class EditorialDisplay: public Actor
{
private:
    std::shared_ptr<GameInfo> _gameInfo;
    bool _inflating = false;
    bool _inflated = false;
    bool _highlighted = false;

    spWebImage _image;
    spTextField _headline;
    spTextField _subText;

public:
    std::shared_ptr<GameInfo> getGameInfo() const { return _gameInfo; }
    spWebImage getImage() const { return _image; };

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

    /*
        Are we highlighed?
     */
    bool isHighlighted() const {return _highlighted;}
    void setHighlight(const bool h);

    /*
    helper for position tweens
     */
    void moveTo(const float x, const float y);

};
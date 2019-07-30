/*
EditorialMenu.cpp
Manage the display elements on screen
includes managment of scrolling behavior and asset load/unload
A cache could be implemented here but I'm not doing it
 */
#include "EditorialMenu.h"

extern unsigned int DEFAULT_IMG_WIDTH;
extern unsigned int DEFAULT_IMG_HEIGHT;

void EditorialMenu::doUpdate(const UpdateState& us)
{
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

    if(_dirty)
        arrangeDisplays();

    return Actor::doUpdate(us);
}

void EditorialMenu::addDisplay(std::shared_ptr<GameInfo> gameInfo)
{
    spEditorialDisplay d = new EditorialDisplay(gameInfo);
    _displays.push_back(d);
    addChild(d);
    // queue the display for async asset fetching
    //_assetQueue.push_back(d);

    _dirty = true;
}
void EditorialMenu::moveLeft()
{
    _dirty = true;
    if(_highlight == 0)
    {
        _highlight = static_cast<unsigned int>(_displays.size() - 1);
    }else{
        _highlight--;
    }
}

void EditorialMenu::moveRight()
{
    _dirty = true;
    if(_highlight == _displays.size() -1)
    {
        _highlight = 0;
    }else{
        _highlight++;
    }
}

void EditorialMenu::arrangeDisplays()
{
    if(!_displays.size())
        return;

    //printf("Arranging displays for _highlight: %u\n", _highlight);

    // simple Layout algorith will go like this:
    // current highlighted menu item is at the center of menu (position(0,0)
    // display visible on either side wrap around circularly
    // Only those displays onscreen will have assets loaded
    // possibly unload when go off screen (or when resource limits reached)

    float x = 0;
    float y = 0;
    float screenWidth = getStage()->getWidth() / 2.0f + DEFAULT_IMG_WIDTH * 2.0f;
    bool onscreen = true;
    int start = _highlight;
    const int numDisplays = static_cast<int>(_displays.size());
    for(int index = start;;)
    {
        auto d = _displays.at(index);
        d->moveTo(x,y);
        d->setHighlight(index == _highlight);

        x += DEFAULT_IMG_WIDTH;
        onscreen = x < screenWidth;
        if(onscreen)
        {
            if(!d->inflated())
                _assetQueue.push_back(d);
        }
        else
            break;
        
        // TODO: we could release assets for widgets offscreen or manage cache?

        ++index;
        if(index == numDisplays) index = 0;
        if(index == start) break;
    }
    int end = _highlight > 0 ? _highlight - 1 : numDisplays -1 ;
    x = 0;
    for(int index = end;;)
    {
        x -= DEFAULT_IMG_WIDTH;
        onscreen = x > (-1.0f*screenWidth);

        auto d = _displays.at(index);

        if(onscreen)
        {
            if(!d->inflated())
                _assetQueue.push_back(d);
        }
        else
            break;

        d->moveTo(x,y);
        d->setHighlight(false);

        --index;
        if(index < 0) index = numDisplays - 1;
        if(index == end) break;
    }    

    _dirty = false;
}

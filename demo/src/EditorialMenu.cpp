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
    if(_dirty)
        arrangeDisplays();

    return Actor::doUpdate(us);
}

void EditorialMenu::addDisplay(std::shared_ptr<GameInfo> gameInfo)
{
    spEditorialDisplay d = new EditorialDisplay(gameInfo);
    _displays.push_back(d);
    addChild(d);

    _dirty = true;
}

void EditorialMenu::clear()
{
    for(auto d:_displays)
    {
        removeChild(d);
    }
    _displays.clear();
    _highlight = 0;
    _cleared = true;
}

void EditorialMenu::moveLeft()
{
    if(_showingDetails) return;

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
    if(_showingDetails) return;

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
        d->moveTo(x, y, !_cleared);
        d->setHighlight(index == _highlight);

        x += DEFAULT_IMG_WIDTH;
        onscreen = x < screenWidth;
        if(onscreen)
        {
            d->inflate();
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
            d->inflate();
        }
        else
            break;

        d->moveTo(x, y, !_cleared);
        d->setHighlight(false);

        --index;
        if(index < 0) index = numDisplays - 1;
        if(index == end) break;
    }    

    _cleared = false;
    _dirty = false;
}

void EditorialMenu::showHideDetails()
{
    if(!_displays.size())
        return;

    spEditorialDisplay d = _displays[_highlight];

    #if 0
    // if the current highlighted display hasn't fetched its assets, forget about it
    // TODO: we should better manage this so that detail window can be opened when assets "in flight"
    if(!_displays[_highlight]->inflated())
        return;
    #endif

    // lazy init
    if(!_details)
    {
        _details = new EditorialDetailsDisplay();
        // Adding element to the stage so it won't pick up anchoring/position of this menu
        // is this the best decision?
        getStage()->addChild(_details);
    }

    if(!_showingDetails)
    {
        _details->show(d);
        _showingDetails = true;

    }else{
        _details->hide();
        _showingDetails = false;
    }
}

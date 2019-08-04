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
    if(_dirtyState != DIRTY_STATE::CLEAN)
        arrangeDisplays();

    return Actor::doUpdate(us);
}

void EditorialMenu::addDisplay(std::shared_ptr<GameInfo> gameInfo)
{
    spEditorialDisplay d = new EditorialDisplay(gameInfo);
    _displays.push_back(d);
    addChild(d);

    _dirtyState = DIRTY_STATE::CLEAR;
}

void EditorialMenu::clear()
{
    if(_details && _showingDetails)
    {
        _details->hide();
        _showingDetails = false;
    }

    for(auto d:_displays)
    {
        removeChild(d);
    }
    _displays.clear();
    _highlight = 0;

    _dirtyState = DIRTY_STATE::CLEAR;
}

void EditorialMenu::moveLeft()
{
    if(_showingDetails) return;

    _dirtyState = DIRTY_STATE::MOVE_LEFT;

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

    _dirtyState = DIRTY_STATE::MOVE_RIGHT;

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
    // Only animate if we're not setting up first time
    bool animate = _dirtyState != DIRTY_STATE::CLEAR;
    int start = _highlight;
    const int numDisplays = static_cast<int>(_displays.size());
    const int numDisplayRight = numDisplays / 2;
    const int numDisplaysLeft = numDisplays - numDisplayRight;
    int displayCount = 0;

    for(int index = start;;)
    {
        auto d = _displays.at(index);

        // adjust edge cases to improve smooth animation
        if(_dirtyState == DIRTY_STATE::MOVE_LEFT && displayCount == numDisplayRight)
        {
            d->setPosition(x - DEFAULT_IMG_WIDTH, y);
        }
        if(_dirtyState == DIRTY_STATE::MOVE_RIGHT && displayCount == numDisplayRight - 1)
        {
            d->setPosition(x + DEFAULT_IMG_WIDTH, y);
        }

        d->moveTo(x, y, animate);
        d->setHighlight(index == _highlight);

        onscreen = (x >= screenWidth * -1.0f) || x <= screenWidth;
        
        if(onscreen)
        {
            // this should be idempotent, so we can
            // safey call multiple times but the asset
            // will only be loaded once
            d->inflate();
        }

        x += DEFAULT_IMG_WIDTH;

        ++index;
        ++displayCount;

        if(displayCount == numDisplayRight)
        {
            x = -1.0f * numDisplaysLeft * DEFAULT_IMG_WIDTH;
        }

        if(index == numDisplays) index = 0;
        if(index == start) break;
    }

    _dirtyState = DIRTY_STATE::CLEAN;
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

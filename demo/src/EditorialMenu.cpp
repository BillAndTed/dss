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
    _assetQueue.push_back(d);

    _dirty = true;
}
void EditorialMenu::moveLeft()
{
    _dirty = true;
    if(_highlight == 0)
    {
        _highlight = _displays.size() - 1;
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

    unsigned int screenW = getStage()->getWidth();
    unsigned int numDisplays = static_cast<unsigned int>(_displays.size());
    unsigned int pos = 0;
    for(auto d = _displays.begin(); d != _displays.end(); ++d)
    {
        (*d)->setPosition(pos * DEFAULT_IMG_WIDTH, 0);
        (*d)->setHighlight(pos == _highlight);        
        pos++;
    }
    

    _dirty = false;
}

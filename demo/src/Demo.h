#pragma once

#include "oxygine-framework.h"
#include "EditorialData.h"
#include "EditorialDisplay.h"

#include <deque>

using namespace oxygine;

DECLARE_SMART(Demo, spDemo)
class Demo: public Actor
{
private:
    // colored bg for debugging
    #if 1
    spColorRectSprite _root;
    #else
    spActor _root;
    #endif

    // pure data cache
    std::shared_ptr<EditorialData> _parsedEditorialData;
    // gui widgets driven by data
    //std::list<spEditorialDisplay> _gameDisplays;
    std::deque<spEditorialDisplay> _assetQueue;

public:

    virtual void onAdded2Stage() override;
    virtual void doUpdate(const UpdateState& us) override;

    void fetchEditorialData();
    void parseEditorialData(const std::string& data);

    void setBackground();
};

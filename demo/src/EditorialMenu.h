/*
EditorialMenu.h
*/
#pragma once

#include "oxygine-framework.h"
#include "EditorialDisplay.h"
#include "EditorialDetailsDisplay.h"

#include <deque>

using namespace oxygine;

DECLARE_SMART(EditorialMenu, spEditorialMenu)
class EditorialMenu: public Actor
{
private:
    std::deque< spEditorialDisplay > _displays;
    std::deque< spEditorialDisplay > _assetQueue;
    unsigned int _highlight = 0;
    bool _dirty = false;
    bool _showingDetails = false;
    spEditorialDetailsDisplay _details;

public:
    virtual void doUpdate(const UpdateState& us) override;

    void addDisplay(std::shared_ptr<GameInfo> gameInfo);
    void moveLeft();
    void moveRight();
    void showHideDetails();

private:
    void arrangeDisplays();
};

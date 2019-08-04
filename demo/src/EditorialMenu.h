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
    enum DIRTY_STATE
    {
        CLEAN = 0,
        CLEAR,
        MOVE_RIGHT,
        MOVE_LEFT
    };

    std::deque< spEditorialDisplay > _displays;
    unsigned int _highlight = 0;
    DIRTY_STATE _dirtyState = CLEAR;
    // Whether a details window is up
    bool _showingDetails = false;
    spEditorialDetailsDisplay _details;

public:
    virtual void doUpdate(const UpdateState& us) override;

    void addDisplay(std::shared_ptr<GameInfo> gameInfo);
    void clear();
    void moveLeft();
    void moveRight();
    void showHideDetails();

private:
    void arrangeDisplays();
};

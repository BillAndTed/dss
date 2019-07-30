/*
EditorialMenu.h
*/
#pragma once

#include "EditorialDisplay.h"

#include <deque>

using namespace oxygine;

DECLARE_SMART(EditorialMenu, spEditorialMenu)
class EditorialMenu: public Actor
{
private:
    Actor _root;
    std::deque< spEditorialDisplay > _displays;
    std::deque< spEditorialDisplay > _assetQueue;
    unsigned int _highlight = 0;
    bool _dirty = false;

public:
    virtual void doUpdate(const UpdateState& us) override;

    void addDisplay(std::shared_ptr<GameInfo> gameInfo);
    void moveLeft();
    void moveRight();

private:
    void arrangeDisplays();
};

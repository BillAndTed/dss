/*
    EditorialDetailsDisplay.h
 */
#pragma once

#include "oxygine-framework.h"

#include "EditorialDisplay.h"

using namespace oxygine;

DECLARE_SMART(EditorialDetailsDisplay, spEditorialDetailsDisplay)
class EditorialDetailsDisplay: public Actor
{
private:
    spColorRectSprite _bg;
    spWebImage _image;
    spTextField _date;
    spTextField _headline;
    spTextField _subText;
    spTextField _blurb;

public:
    //virtual void doUpdate(const UpdateState& us) override;

    void show(spEditorialDisplay display);
    void hide();
};
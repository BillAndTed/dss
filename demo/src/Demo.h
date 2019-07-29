#include "oxygine-framework.h"

using namespace oxygine;

DECLARE_SMART(Demo, spDemo)
class Demo: public Actor
{
private:
    spTextField _text;
    spSprite    _button;

public:

    virtual void onAdded2Stage() override;

    void setBackground();
};

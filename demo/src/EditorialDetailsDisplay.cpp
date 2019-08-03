#include "EditorialDetailsDisplay.h"

extern unsigned int DEFAULT_IMG_WIDTH;
extern unsigned int DEFAULT_IMG_HEIGHT;
static const unsigned int DETAILS_BORDER = 20;

void EditorialDetailsDisplay::show(spEditorialDisplay display)
{
    // Simple layout from top to bottom
    float y = DETAILS_BORDER;
    float x = DETAILS_BORDER;
    float w = DEFAULT_IMG_WIDTH * 2 + DETAILS_BORDER * 2;
    float h = getStage()->getHeight();

    // lazy init most of this stuff on first show
    if(!_bg)
    {
        _bg = new ColorRectSprite();
        _bg->setColor(Color::Gray);
        _bg->setWidth(w);
        _bg->setHeight(h);
        _bg->setPosition(0,0);

        // add all to the scene graph
        addChild(_bg);
    }

    // always pick up images/data from display on show
    // TODO: we could do this only on change or release everything when hiding
    // TODO: fetch a better quality image when menu shown
    _image = display->getImage()->clone();
    _image->setPosition(DETAILS_BORDER, DETAILS_BORDER);
    _image->setScale(2.0, 2.0);
    _bg->addChild(_image);

    // spTextField _date;
    // spTextField _headline;
    // spTextField _subText;
    // spTextField _blurb;

    // actually show the display
    // controlling visibility helps not make draw calls when offscreen
    setVisible(true);
    setPriority(1000);
    #if 0
    setPosition(getStage()->getWidth() - w, 0);
    #else
    // fade and move in
    setAlpha(0);
    addTween(Actor::TweenAlpha(255), 500);
    float posX = getStage()->getWidth() - w;
    float posY = 0;
    addTween(Actor::TweenPosition(Vector2(posX,posY)), 300);
    #endif
}

void EditorialDetailsDisplay::hide()
{
    #if 0
    setVisible(false);
    #else
    setAlpha(255);
    spTween alphaTween = addTween(Actor::TweenAlpha(0), 500);
    float posX = getStage()->getWidth();
    float posY = 0;
    spTween posTween = addTween(Actor::TweenPosition(Vector2(posX,posY)), 300);
    // controlling visibility helps not make draw calls when offscreen
    spEditorialDetailsDisplay self = this;
    alphaTween->addEventListener(Event::COMPLETE, [self](Event* event){
        self->setVisible(false);
    });
    #endif
}
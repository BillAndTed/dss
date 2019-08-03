#include "EditorialDetailsDisplay.h"

extern unsigned int DEFAULT_IMG_WIDTH;
extern unsigned int DEFAULT_IMG_HEIGHT;
static const unsigned int DETAILS_BORDER = 10;
static const unsigned int HEADLINE_HEIGHT = 40;
static const unsigned int LARGE_TEXT = 36;
static const unsigned int MED_TEXT = 24;
static const unsigned int SMALL_TEXT = 18;
static const std::string DEFAULT_FONT = "freeSans";

extern Resources gameResources;

// helper
spTextField createTextField(const int width, const int height, const int fontSize, const std::string& font)
{
    spTextField t = new TextField();
    t->setFont(gameResources.getResFont(font));
    t->setWidth(width);
    t->setHeight(height);
    t->setMultiline(true);
    t->setBreakLongWords(true);
    t->setFontSize(fontSize);
    t->setColor(Color::White);

    return t;
}

void shiftLayoutDown(float& y, spTextField t)
{
    RectF rect;
    t->getBounds(rect);
    y += rect.getHeight() + DETAILS_BORDER;
}

void EditorialDetailsDisplay::show(spEditorialDisplay display)
{
    // Simple layout from top to bottom
    float y = DETAILS_BORDER;
    float x = DETAILS_BORDER;
    float w = DEFAULT_IMG_WIDTH * 3;
    float h = getStage()->getHeight();

    // lazy init most of this stuff on first show
    if(!_bg)
    {
        _bg = new ColorRectSprite();
        _bg->setColor(Color::DarkSlateGray);
        _bg->setWidth(w + DETAILS_BORDER * 2);
        _bg->setHeight(h);
        _bg->setPosition(0,0);

        // add all to the scene graph
        addChild(_bg);
    }

    if(!_headline)
    {
        _headline = createTextField(w, HEADLINE_HEIGHT, LARGE_TEXT, DEFAULT_FONT);
        _headline->setPosition(x, y);
        _bg->addChild(_headline);
    }
    _headline->setText(display->getGameInfo()->_headline);
    shiftLayoutDown(y, _headline);

    // always pick up imagesfrom display on show
    // TODO: we could do this only on change or release everything when hiding
    // TODO: fetch a better quality image when menu shown
    _image = display->getImage()->clone();
    _image->setAnchor(0,0);
    _image->setPosition(x, y);
    _image->setScale(2.0, 2.0);
    _bg->addChild(_image);

    y += DEFAULT_IMG_HEIGHT * 2 + DETAILS_BORDER;

    if(!_date)
    {
        _date = createTextField(w, HEADLINE_HEIGHT, SMALL_TEXT, DEFAULT_FONT);
        _date->setPosition(x, y);
        _bg->addChild(_date);
    }
    _date->setText(display->getGameInfo()->_date);
    shiftLayoutDown(y, _date);

    if(!_subText)
    {
        _subText = createTextField(w, HEADLINE_HEIGHT, MED_TEXT, DEFAULT_FONT);
        _subText->setPosition(x, y);
        _bg->addChild(_subText);
    }
    _subText->setText(display->getGameInfo()->_subHead);
    shiftLayoutDown(y, _subText);

    if(!_blurb)
    {
        _blurb = createTextField(w, HEADLINE_HEIGHT, SMALL_TEXT, DEFAULT_FONT);
        _blurb->setPosition(x, y);
        _bg->addChild(_blurb);
    }
    _blurb->setText(display->getGameInfo()->_blurb);

    // actually show the display
    // controlling visibility helps not make draw calls when offscreen
    setVisible(true);
    setPriority(1000);
    #if 0
    setPosition(getStage()->getWidth() - w, 0);
    #else
    // fade and move in
    setPosition(getStage()->getWidth(), 0);
    setAlpha(0);
    addTween(Actor::TweenAlpha(255), 500);
    float posX = getStage()->getWidth() - w - 2 * DETAILS_BORDER;
    float posY = 0;
    addTween(Actor::TweenPosition(Vector2(posX,posY)), 300);
    #endif
}

void EditorialDetailsDisplay::hide()
{
    // TODO: maybe we should release everything when hidden if we're constrained for system resources?

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
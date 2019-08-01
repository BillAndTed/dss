#include "EditorialDisplay.h"

#include <math.h> // for fabs

extern unsigned int DEFAULT_IMG_WIDTH;
extern unsigned int DEFAULT_IMG_HEIGHT;
static const unsigned int HEADLINE_POS_Y = 40;
static const unsigned int SUBTEXT_POS_Y = 20;
static const unsigned int HEADLINE_HEIGHT = 40;
static const std::string DEFAULT_FONT = "freeSans";

extern Resources gameResources;

EditorialDisplay::EditorialDisplay(std::shared_ptr<GameInfo> gameInfo)
    :_gameInfo(gameInfo)
{
    setWidth(DEFAULT_IMG_WIDTH);
    setHeight(DEFAULT_IMG_HEIGHT);
    setAnchor(0.5, 0.5);
}

void EditorialDisplay::inflate()
{
    _inflating = true;
    _inflated = false;

    // async fetch of image off network
    _image = new WebImage;
    addChild(_image);

    _image->load(_gameInfo->_imgURL);
    
    _image->setTouchEnabled(false);
    _image->setSize(_gameInfo->_imgWidth, _gameInfo->_imgHeight);
    _image->setX(0);
    _image->setY(0);

    // add bottom and top text (not visible unless focused)
    _headline = new TextField();
    _headline->setPosition(0, 0.0f - HEADLINE_POS_Y);
    _headline->setWidth(DEFAULT_IMG_WIDTH);
    _headline->setHeight(HEADLINE_HEIGHT);
    _headline->setMultiline(false);
    _headline->setBreakLongWords(false);
    _headline->setFontSize(24);

    TextStyle style = TextStyle(gameResources.getResFont(DEFAULT_FONT)).withColor(Color::White).alignMiddle();
    _headline->setStyle(style);
    _headline->setText(_gameInfo->_headline);
    addChild(_headline);

    _subText = new TextField();
    _subText->setPosition(0, DEFAULT_IMG_HEIGHT + SUBTEXT_POS_Y);
    _subText->setWidth(DEFAULT_IMG_WIDTH);
    _subText->setHeight(HEADLINE_HEIGHT);
    _subText->setVAlign(TextStyle::VALIGN_TOP);
    _subText->setFontSize(16);

    _subText->setStyle(style);
    _subText->setMultiline(true);
    _subText->setBreakLongWords(true);
    _subText->setText(_gameInfo->_subHead);
    addChild(_subText);


    spEditorialDisplay self = this;
    std::shared_ptr<GameInfo> _info = _gameInfo;

    printf("Initiating async fetch of asset %s...\n", _gameInfo->_imgURL.c_str());

    _image->addEventListener(HttpRequestTask::COMPLETE, [ self, _info](Event* event){

        printf("Async fetch of asset %s completed...\n", _info->_imgURL.c_str());

        self->_inflating = false;
        self->_inflated = true;

        #if 1
        // fade in the asset
        self->setAlpha(0);
        self->addTween(Actor::TweenAlpha(255), 500);
        #endif
        
        self->_image->removeAllEventListeners();
    });

    _image->addEventListener(HttpRequestTask::ERROR, [self, _info](Event* event){

        self->_inflating = false;
        self->_inflated = true;
        
        self->_image->removeAllEventListeners();
    });

    setHighlight(_highlighted);
}

void EditorialDisplay::deflate()
{
    _inflating = false;
    _inflated = false;
}

void EditorialDisplay::setHighlight(const bool h)
{
    _highlighted = h;

    if(_highlighted)
    {
        #if 1
        if(getScaleX() < 1.5f)
            addTween(TweenScale(1.5f), 300);
        #else
        setScale(1.5f, 1.5f);
        #endif
        setPriority(200);

    }else{
        #if 1
        if(getScaleX() > 1.0f)
            addTween(TweenScale(1.0f), 300);
        #else
        setScale(1.0f, 1.0f);
        #endif
        setPriority(100);
    }
    unsigned char startAlpha = _highlighted ? 0 : 255; 
    unsigned char endAlpha = 255 - startAlpha;
    if(_headline)
    {
        #if 1
        _headline->setAlpha(startAlpha);
        _headline->addTween(Actor::TweenAlpha(endAlpha), 500);
        #endif
        _headline->setVisible(_highlighted);
    }

    if(_subText)
    {
        #if 1
        _subText->setAlpha(startAlpha);
        _subText->addTween(Actor::TweenAlpha(endAlpha), 500);
        #endif
        _subText->setVisible(_highlighted);

    }
}

void EditorialDisplay::moveTo(const float x, const float y)
{
    #if 1
    const float dx = std::fabs(getX() - x);
    if(dx < 2.0f * DEFAULT_IMG_WIDTH)
        addTween(Actor::TweenPosition(Vector2(x,y)), 300);
    else
        setPosition(x,y);
    #else
    setPosition(x,y);
    #endif
}

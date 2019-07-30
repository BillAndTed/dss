#include "EditorialDisplay.h"

extern unsigned int DEFAULT_IMG_WIDTH;
extern unsigned int DEFAULT_IMG_HEIGHT;
static const unsigned int HEADLINE_POS_Y = 40;
static const unsigned int HEADLINE_HEIGHT = 40;

extern Resources gameResources;

EditorialDisplay::EditorialDisplay(std::shared_ptr<GameInfo> gameInfo)
    :_gameInfo(gameInfo)
{
    _root = new ColorRectSprite();
    _root->setWidth(DEFAULT_IMG_WIDTH);
    _root->setHeight(DEFAULT_IMG_HEIGHT);
    _root->setColor(Color::White);
    _root->setAnchor(0.5, 0.5);
    addChild(_root);
}

void EditorialDisplay::inflate()
{
    _inflating = true;
    _inflated = false;

    // async fetch of image off network
    _image = new WebImage;
    _root->addChild(_image);

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

    TextStyle style = TextStyle(gameResources.getResFont("main")).withColor(Color::White).alignMiddle();
    _headline->setStyle(style);
    _headline->setText(_gameInfo->_headline);
    _root->addChild(_headline);

    _subText = new TextField();
    _subText->setPosition(0, DEFAULT_IMG_HEIGHT);
    _subText->setWidth(DEFAULT_IMG_WIDTH * 2.0);
    _subText->setHeight(HEADLINE_HEIGHT * 2.0);

    _subText->setStyle(style);
    #if 0
    printf("--> subtext is: %s\n", _gameInfo->_subHead.c_str());
    #endif
    _subText->setText(_gameInfo->_subHead);
    _root->addChild(_subText);


    spEditorialDisplay self = this;
    std::shared_ptr<GameInfo> _info = _gameInfo;

    printf("Initiating async fetch of asset %s...\n", _gameInfo->_imgURL.c_str());

    _image->addEventListener(HttpRequestTask::COMPLETE, [self, _info](Event* event){

        printf("Async fetch of asset %s completed...\n", _info->_imgURL.c_str());

        self->_inflating = false;
        self->_inflated = true;

        #if 1
        // fade in the asset
        self->setAlpha(0);
        self->addTween(Actor::TweenAlpha(255), 500);
        #endif
    });

    _image->addEventListener(HttpRequestTask::ERROR, [self, _info](Event* event){

        self->_inflating = false;
        self->_inflated = true;
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
        if(_root->getScaleX() < 1.5f)
            _root->addTween(TweenScale(1.5f), 300);
        #else
        setScale(1.5f, 1.5f);
        #endif
        setPriority(200);

    }else{
        #if 1
        if(_root->getScaleX() > 1.0f)
            _root->addTween(TweenScale(1.0f), 300);
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
    _root->addTween(Actor::TweenPosition(Vector2(x,y)), 300);
    #else
    _root->setPosition(x,y);
    #endif
}
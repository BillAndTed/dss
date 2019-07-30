#include "EditorialDisplay.h"

extern unsigned int DEFAULT_IMG_WIDTH;
extern unsigned int DEFAULT_IMG_HEIGHT;

EditorialDisplay::EditorialDisplay(std::shared_ptr<GameInfo> gameInfo)
    :_gameInfo(gameInfo)
{
    _root = new ColorRectSprite();
    _root->setWidth(DEFAULT_IMG_WIDTH);
    _root->setHeight(DEFAULT_IMG_HEIGHT);
    _root->setColor(Color::Red);
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
    //sp->setPriority(110);

    spEditorialDisplay self = this;
    std::shared_ptr<GameInfo> _info = _gameInfo;

    printf("Initiating async fetch of asset %s...\n", _gameInfo->_imgURL.c_str());

    _image->addEventListener(HttpRequestTask::COMPLETE, [self, _info](Event* event){

        printf("Async fetch of asset %s completed...\n", _info->_imgURL.c_str());

        self->_inflating = false;
        self->_inflated = true;
    });

    _image->addEventListener(HttpRequestTask::ERROR, [self, _info](Event* event){

        self->_inflating = false;
        self->_inflated = true;
    });
}

void EditorialDisplay::deflate()
{
    _inflating = false;
    _inflated = false;
}
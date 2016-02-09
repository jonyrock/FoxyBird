#include "BackgroundAsset.h"
#include "Models/FieldModel.h"
#include "Utils.h"
#include "SimpleAudioEngine.h"


using namespace cocos2d;


BackgroudAsset* BackgroudAsset::create(const FieldModel* fieldModel) {
    auto pRet = new BackgroudAsset();
    if (pRet && pRet->init(fieldModel)) { pRet->autorelease(); return pRet; }
    else { delete pRet; pRet = NULL; return NULL; }
}

bool BackgroudAsset::init(const FieldModel* fieldModel) {
    if(!Node::init()) {
        return false;
    }
    _fieldModel = fieldModel; 
    _size = Director::getInstance()->getVisibleSize();
    _size.height *= (1 - FieldModel::TARGET_YSCALE);
    setContentSize(_size);
    initBackground();
    initShadows();
    initStars();
    initMoon();
    initMountains();
    scheduleUpdate();
    return true;
}

void BackgroudAsset::initBackground() {
    Size halfSize(_size.width / 2, _size.height);
    // we will change this colors in the runtime
    _backGradLeft = LayerGradient::create(Color4B::WHITE,Color4B::WHITE, Vec2(1,0));
    _backGradRight = LayerGradient::create(Color4B::WHITE,Color4B::WHITE, Vec2(-1,0));
    _backGradLeft->setContentSize(halfSize);
    _backGradLeft->setPosition(_size/-2);
    _backGradRight->setContentSize(halfSize);
    _backGradRight->setPosition(Vec2(0, -_size.height / 2));
    addChild(_backGradLeft, 1);
    addChild(_backGradRight, 1);
}

void BackgroudAsset::initStars() {
    auto sprite = Sprite::create("stars.png");
    addChild(sprite, 4);
    sprite->setOpacity(140);
    Utils::setNodeHeightR(sprite, 0.8);
    sprite->setAnchorPoint(Vec2(0, 1));
    Vec2 offset = Vec2(-_size.width / 2,_size.height/2);
    sprite->setPosition(offset);
    float itemWidth = sprite->getContentSize().width * sprite->getScaleX();
    int itemsCount = (int)ceil(_size.width / itemWidth);
    for(int i = 1; i < itemsCount; i++) {
	offset.x += itemWidth;
	auto sprite = Sprite::create("stars.png");
	sprite->setOpacity(140);
	sprite->setAnchorPoint(Vec2(0, 1));
	addChild(sprite, 5);
	sprite->setPosition(offset);
	Utils::setNodeHeightR(sprite, 0.8);
    }
}

void BackgroudAsset::initMoon() {
    auto sprite = Sprite::create("moon.png");
    sprite->setPosition(0, -_size.height / 2 + 17);
    Utils::setNodeHeightR(sprite, 0.18);
    sprite->setAnchorPoint(Vec2(0.5, 0));
    addChild(sprite, 5);
}


void BackgroudAsset::initMountains() {
    auto sprite = Sprite::create("mountain.png");
    auto spriteO = Sprite::create("mountain_light.png");
    //spriteO->setBlendFunc(BlendFunc::ADDITIVE);
    _mountainsSprites.push_back(sprite);
    _mountainsSpritesLight.push_back(spriteO);
    addChild(sprite, 5);
    addChild(spriteO, 5);
    Utils::setNodeHeightR(sprite, 0.72);
    Utils::setNodeHeightR(spriteO, 0.72);
    Vec2 offset = (_size - sprite->getContentSize() * 
	sprite->getScale()) / -2;
    sprite->setPosition(offset);
    spriteO->setPosition(offset);
    float itemWidth = sprite->getContentSize().width * sprite->getScaleX();
    int itemsCount = (int)ceil(_size.width / itemWidth);
    for(int i = 1; i < itemsCount; i++) {
	offset.x += itemWidth;
	auto sprite = Sprite::create("mountain.png");
	auto spriteO = Sprite::create("mountain_light.png");
	//spriteO->setBlendFunc(BlendFunc::ADDITIVE);
	spriteO->setColor(Color3B::GREEN);
	_mountainsSprites.push_back(sprite);
	_mountainsSpritesLight.push_back(spriteO);
	addChild(sprite, 6);
	addChild(spriteO, 6);
	sprite->setPosition(offset);
	spriteO->setPosition(offset);
	Utils::setNodeHeightR(sprite, 0.72);
	Utils::setNodeHeightR(spriteO, 0.72);
    }
}

void BackgroudAsset::initShadows() {
    auto spriteLeft = Sprite::create("shadow_corner.png");
    auto spriteRight = Sprite::create("shadow_corner.png");
    addChild(spriteLeft, 3);
    addChild(spriteRight, 3);
    spriteLeft->setAnchorPoint(Vec2(0,1));
    spriteRight->setAnchorPoint(Vec2(1,1));
    spriteLeft->setPosition(Vec2(_size.width/-2, _size.height/2));
    spriteRight->setPosition(Vec2(_size.width/2, _size.height/2));
    Utils::setNodeHeightR(spriteLeft, 3);
    Utils::setNodeHeightR(spriteRight, 3);
    spriteLeft->setFlippedX(true);
}

void BackgroudAsset::update(float) {
    _backGradLeft->setStartColor(Color3B(_fieldModel->colorSky2));
    _backGradLeft->setEndColor(Color3B(_fieldModel->colorSky1));
    _backGradRight->setStartColor(Color3B(_fieldModel->colorSky2));
    _backGradRight->setEndColor(Color3B(_fieldModel->colorSky1));
    for(auto p: _mountainsSprites) {
        p->setColor(Color3B(_fieldModel->colorMountains));
    }
    for(auto p: _mountainsSpritesLight) {
        p->setColor(Color3B(_fieldModel->colorMountainsLight));
    }
    
}

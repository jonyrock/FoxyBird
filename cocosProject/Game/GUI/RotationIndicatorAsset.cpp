#include "RotationIndicatorAsset.h"
#include "2d/CCNode.h"
#include "2d/CCSprite.h"

using namespace cocos2d;

bool RotationIndicatorAsset::init() {
    if(!Node::init()) return false;
    initSprite();
    return true;
}

void RotationIndicatorAsset::initSprite() {
    _sprite = Sprite::create("clock_hand.png");
    _sprite->setAnchorPoint(Vec2(0.863, 0.5));
    _sprite->setRotation(90);
    _sprite->setScale(0.3);
    addChild(_sprite);
}

void RotationIndicatorAsset::setInfoRotation(float a) {
    _sprite->setRotation(90 - a);
}



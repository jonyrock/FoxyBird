#include "TutorialArrowAsset.h"
#include "Utils.h"

using namespace cocos2d;

bool TutorialArrowAsset::init() {
    if(!Node::init()) return false;
    initSprite();
    initAnimation();
    return true;
}

void TutorialArrowAsset::initSprite() {
    _sprite = Sprite::create("tutorial-arrow.png");
    _sprite->setAnchorPoint(Vec2(1 - 0.333, 1 - 0.102));
    _sprite->setPosition(Vec2(0, 0));
    _sprite->setRotation(200);
    _sprite->setFlippedX(true);
    Utils::setNodeHeightR(_sprite, 0.15);
    addChild(_sprite, 1);
}

void TutorialArrowAsset::initAnimation() {
    _sprite->runAction(RepeatForever::create(
	Sequence::create(
	    MoveTo::create(0.3, Vec2(0, 13)), 
	    MoveTo::create(0.2, Vec2(0, 0)),
	    nullptr
	)
    ));
}


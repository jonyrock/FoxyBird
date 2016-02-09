#include "StageProgressAsset.h"
#include "Utils.h"

using namespace cocos2d;

bool StageProgressAsset::init() {
    if(!Node::init()) return false;
    _size = Director::getInstance()->getVisibleSize();
    initBack();
    initLine();
    initIcon();
    setContentSize(_back->getContentSize());
    setAnchorPoint(Vec2(1,1));
    return true;
}

void StageProgressAsset::initBack() {
    auto _backBorder = LayerColor::create(
	Color4B(255,255,255, 255 * 0.15), 
	_size.width * 0.217 + 1.5 * 2, _size.height * 0.01 + 1.5 * 2
    ); 
    _backBorder->setPosition(Vec2(-1.5,-1.5));
    addChild(_backBorder, 1);
    auto backColor = Color4B::BLACK;
    backColor.a = 102;
    _back = LayerColor::create(
	backColor, _size.width * 0.217, _size.height * 0.01
    );
    addChild(_back, 1);
}

void StageProgressAsset::initLine() {
    Size s = _back->getContentSize();
    _targetLineWidth = s.width;
    _line = LayerColor::create(Color4B::YELLOW, s.width, s.height);
    _line->setPosition(0,0);
    setProgress(0);
    addChild(_line, 2);
}

void StageProgressAsset::initIcon() {
    _icon = Sprite::create("ic_star.png");
    //_icon->setScale(0.25);
    _icon->setPosition(_targetLineWidth + 2, 3);
    Utils::setNodeWidthR(_icon, 0.033);
    addChild(_icon, 2);
}

void StageProgressAsset::setProgress(float h) {
    float w = _targetLineWidth * h / 100;
    _line->setContentSize(Size(w, _line->getContentSize().height));
}

void StageProgressAsset::onNextStage() {
    float fromScale = _icon->getScale();
    _icon->runAction(Sequence::create(
	ScaleTo::create(0.1, fromScale + 0.3), 
	ScaleTo::create(0.1, fromScale),
	ScaleTo::create(0.1, fromScale + 0.3), 
	ScaleTo::create(0.1, fromScale),
	nullptr
    ));
}



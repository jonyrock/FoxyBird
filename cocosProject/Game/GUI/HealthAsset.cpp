#include "HealthAsset.h"
#include "Utils.h"
#include <cocos/ui/UIScale9Sprite.h>


using namespace cocos2d;

bool HealthAsset::init() {
    if(!Node::init()) return false;
    _size = Director::getInstance()->getVisibleSize();
    initBack();
    initLine();
    initIcon();
    setContentSize(_back->getContentSize());
    setAnchorPoint(Vec2(0,1));
    return true;
}

void HealthAsset::initBack() {
//    auto c = cocos2d::ui::Scale9Sprite::create("button.png", Rect(0,0,400,184), Rect(120,0,400,184));
//    c->setContentSize(Size(_size.width / 2.5, _size.height / 22));
//    c->setAnchorPoint(Vec2(0,1));
//    c->setPosition(0,0);
//    addChild(c, 2);
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

void HealthAsset::initLine() {
    Size s = _back->getContentSize();
    _targetLineWidth = s.width;
    _line = LayerColor::create(
	Color4B(Utils::colorFromString("#00ff0a")), 
	s.width, s.height
    );
    _line->setPosition(0,0);
    addChild(_line, 2);
}

void HealthAsset::initIcon() {
    auto s = Sprite::create("ic_health.png");
    s->setPosition(0, 2.5);
    Utils::setNodeWidthR(s, 0.033);
    addChild(s, 3);
}

void HealthAsset::setHealth(float h) {
    float w = _targetLineWidth * h / 100;
    _line->setContentSize(Size(w, _line->getContentSize().height));
}


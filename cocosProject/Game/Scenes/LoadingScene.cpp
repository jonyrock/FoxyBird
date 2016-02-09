#include "LoadingScene.h"
#include "GUI/GUI.h"
#include "Utils.h"

using namespace cocos2d;

bool LoadingScene::init() {
    if(!BasicScene::init()) {
        return false;
    }
    auto s = Sprite::create("loading_scene.png");
    s->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    s->setPosition(_size/2);
    Utils::setNodeHeight(s, _size.height);
    if(s->getContentSize().width * s->getScale() < _size.width) {
        Utils::setNodeWidth(s, _size.width);
    }
    addChild(s, 1);
    return true;
}
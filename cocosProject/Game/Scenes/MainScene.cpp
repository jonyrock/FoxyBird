#include "MainScene.h"
#include "Managers/ScenesManager.h"
#include "Managers/SoundsManager.h"
#include "Managers/LivesManager.h"
#include "Utils.h"
#include "GUI/GUI.h"


using namespace cocos2d;


bool MainScene::init() {
    if(!BasicScene::init()) return false;
    initBack();
    initButtons();
    if(!LivesManager::getInstance()->isUnlimited())
        ScenesManager::getInstance()->showLives();
    ScenesManager::getInstance()->showSettings();
    return true;
}

void MainScene::initBack() {
    auto s = Sprite::create("main_scene_back.png");
    s->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    s->setPosition(_size/2);
    Utils::setNodeHeight(s, _size.height);
    if(s->getContentSize().width * s->getScale() < _size.width) {
	Utils::setNodeWidth(s, _size.width);
    }
    addChild(s, 1);
}

void MainScene::initButtons() {
    auto button = ui::Button::create("btn_play.png", "btn_play_on.png", "btn_play_on.png");
    //button->setScale9Enabled(true);
    button->setAnchorPoint({0.5, 1});
    addChild(button, 2);
    button->setPosition(Utils::coordFromPSD({1136/2, 432}));
    Utils::setNodeHeightR(button, 129.0f/640);
    button->addTouchEventListener([this, button](Ref*, ui::Widget::TouchEventType touchType) {
	if(touchType == ui::Widget::TouchEventType::ENDED) {
            button->setEnabled(false);
	    SoundsManager::getInstance()->playButtonEffect();
	    if(!ScenesManager::getInstance()->gotoGame())
                button->setEnabled(true);
	}
    });
}

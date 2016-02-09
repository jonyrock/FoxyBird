#include "LivesBoxAsset.h"
#include "Managers/LivesManager.h"
#include "Utils.h"
#include <string>
#include <functional>
#include <cocos/ui/UIButton.h>
#include "GUI.h"


using namespace cocos2d;
using namespace std;


bool LivesBoxAsset::init() {
    if(!Node::init()) return false;
    initBack();
    initCounter();
    scheduleUpdate();
    return true;
}

void LivesBoxAsset::initBack() {
    auto sprite = Sprite::create("ic_live.png");
    Utils::setNodeWidthR(sprite, 37.0f/1136);
    auto backSprite = Sprite::create("rounded-rect.png");
    Utils::setNodeWidthR(backSprite, 129.0f/1136);
    setContentSize(backSprite->getContentSize() * backSprite->getScale());
    //lifesNode->setContentSize(backSprite->getContentSize() * backSprite->getScale());
    //lifesNode->setAnchorPoint({0, 1});
    
    auto str = Utils::toString(LivesManager::getInstance()->getLivesAvailable());
    _numberLabel = GUI::createLabelB(str, 34);
    // test set to comtute h scale. It could me any number
    _numberLabel->setString("4");
    addChild(_numberLabel, 4);
    Utils::setNodeHeightR(_numberLabel, 0.9);
    // end test set
    _numberLabel->setColor(Color3B::WHITE);
    
    sprite->setPosition(-sprite->getContentSize().width * sprite->getScale() * 0.82, 0);
    auto coordDiff = Utils::coordFromPSD({44/2,0});
    _numberLabel->setPosition({coordDiff.x,0});
    addChild(backSprite, 3);
    addChild(sprite, 3);
    
    _numberLabel->setString(str);
    
}

void LivesBoxAsset::initCounter() {
    _timeToNextLifeTitleLabel = GUI::createLabelR("NEXT LIFE IN ", 22);
    Utils::setNodeWidthR(_timeToNextLifeTitleLabel, 191.0f/1136);
    _timeToNextLifeTitleLabel->setAnchorPoint({0, 0.5});
    addChild(_timeToNextLifeTitleLabel, 3);
    _timeToNextLifeLabel = GUI::createLabelB("00:00", 30);
    _timeToNextLifeLabel->setScale(_timeToNextLifeTitleLabel->getScale());
    _timeToNextLifeLabel->setAnchorPoint({0, 0.5});
    addChild(_timeToNextLifeLabel, 3);
    
    auto coordDiff = Utils::coordFromPSD({78,0});
    
    _timeToNextLifeTitleLabel->setPosition(
        {coordDiff.x,0}
        //Utils::coordFromPSD({129+30+20, 56 + 13})
    );
	
    _timeToNextLifeLabel->setPosition(
	_timeToNextLifeTitleLabel->getPosition() + Vec2(_timeToNextLifeTitleLabel->getContentSize().width * _timeToNextLifeTitleLabel->getScale(), 0)
    );
    
    if(LivesManager::getInstance()->isFull()) onFull();
}

void LivesBoxAsset::onFull() {
    //_lifesNumberLabel->setVisible(false);
    _timeToNextLifeLabel->setVisible(false);
    _timeToNextLifeTitleLabel->setVisible(false);
    auto fullLabel = GUI::createLabelB("FULL", 30);
    fullLabel->setAnchorPoint({0,0.5});
    fullLabel->setPosition(_timeToNextLifeTitleLabel->getPosition());
    Utils::setNodeWidthR(fullLabel, 85.0f/1136);
    addChild(fullLabel, 3);
}


void LivesBoxAsset::initIcons() {
    
}

void LivesBoxAsset::update(float dt) {
    if(LivesManager::getInstance()->isFull()) {
        onFull();
        unscheduleUpdate();
        return;
    }
    auto strNum = Utils::toString(LivesManager::getInstance()->getLivesAvailable());
    _numberLabel->setString(strNum);
    string strTime;
    LivesManager::getInstance()->getStringTime(strTime);
    _timeToNextLifeLabel->setString(strTime);
}


#include "FullVersionLayer.h"

#include "GUI.h"
#include "ErrorMessageBox.h"
#include "Managers/SoundsManager.h"
#include "Managers/EconomicsManager.h"
#include "Managers/ScenesManager.h"
#include "Utils.h"

#include <cocos/ui/UIButton.h>
#include <string>
#include <iostream>


#define CLOSE_BUTTON_NAME "closeBuyttonButton"
#define UPGRADE_TITLE_NAME "updateTitle"

#define UPDATE_TEXT_POSITION Utils::coordFromPSD({1136/2, 482})
#define STATUS_TEXT_POSITION Utils::coordFromPSD({1136/2, 412})


using namespace cocos2d;
using namespace std;


bool FullVersionLayer::init() {
    if(!LayerColor::initWithColor(Color4B(32, 36, 47, 0.8 * 255)))
	return false;
    initOverlayListener();
    initBackWithClose();
    initTitle();
    initPurchase();
    initRestore();
    return true;
}

// TODO: make it basic (it is used in TutorialBox)
void FullVersionLayer::initOverlayListener() {
    auto l = EventListenerTouchOneByOne::create();
    l->onTouchBegan = [this](Touch*, Event*) { return true; };
    l->setSwallowTouches(true);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(l, this);
}

void FullVersionLayer::initBackWithClose() {
    // close button itited together with back because it's positions depends on
    // back width
    auto back = Sprite::create("full_version_back.png");
    Utils::setNodeHeightR(back, 483.0f/640);
    back->setPosition(Utils::coordFromPSD({1136/2, 49}));
    back->setAnchorPoint({0.5f, 1.0f});
    addChild(back);
    auto b = ui::Button::create("full_version_close_off.png", 
		"full_version_close_on.png", "full_version_close_on.png");
    b->addTouchEventListener([this](Ref*, ui::Widget::TouchEventType touchType) {
	if(touchType == ui::Widget::TouchEventType::ENDED) {
	    SoundsManager::getInstance()->playButtonEffect();
	    this->removeFromParent();
	}
    });
    addChild(b, 2, CLOSE_BUTTON_NAME);
    float xOffset = back->getPosition().x + 0.33 * back->getContentSize().width * back->getScale();
    b->setPosition(Utils::coordFromPSD({0,88}) + Vec2(xOffset,0));
    b->setAnchorPoint({0,1});
    Utils::setNodeHeightR(b, 50.0f/640);
}

void FullVersionLayer::initTitle() {
    auto t = GUI::createLabelI("GET UNLIMITED LIVES AND REMOVE ALL ADS", 13);
    t->setColor(Color3B::BLACK);
    t->setAnchorPoint({0.5, 0.8});
    t->setPosition(Utils::coordFromPSD({1136/2, 240}));
    addChild(t);
}

void FullVersionLayer::initPurchase() {
    _buyButton = ui::Button::create(
        "full_version_button_off.png", 
	"full_version_button_on.png", 
        "full_version_button_lock.png"
    );
    _buyButton->addTouchEventListener([this](Ref*, ui::Widget::TouchEventType touchType) {
	if(touchType == ui::Widget::TouchEventType::ENDED) {
	    SoundsManager::getInstance()->playButtonEffect();
	    onPurchase();
	}
    });
    addChild(_buyButton);
    _buyButton->setPosition(Utils::coordFromPSD({1136/2,448}));
    _buyButton->setAnchorPoint({0.5,1});
    Utils::setNodeHeightR(_buyButton, 92.0f/640);
    auto t = GUI::createLabelBI("UPGRADE", 29);
    t->setPosition(UPDATE_TEXT_POSITION);
    t->setAnchorPoint({0.5,0.8});
    
    addChild(t, 2, UPGRADE_TITLE_NAME);
}

void FullVersionLayer::initRestore() {
    
    float y = _buyButton->getPosition().y - 
	      _buyButton->getContentSize().height * 
	      _buyButton->getScaleY();
    float x = _buyButton->getPosition().x;
    
    float height = y / 1.6;
    float width = _buyButton->getContentSize().width * _buyButton->getScaleX()/2;
    _restoreButton = GUI::TextButton::create(
        Size(width * 1.5, height), "restore", 
        [this]() { onRestore(); }, 
        Color3B::WHITE, Color3B::WHITE, Color3B::WHITE, Color3B::BLACK,
        width * 0.25
    );
    
    _restoreButton->setPosition(Vec2(x, y/2));
    _restoreButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    addChild(_restoreButton, 2);
}

void FullVersionLayer::onBeginOperation() {
    _inProcess = true;
    _buyButton->setEnabled(false);
    _buyButton->setVisible(false);
    _restoreButton->setEnabled(false);
    _restoreButton->setVisible(false);
    getChildByName<ui::Button*>(CLOSE_BUTTON_NAME)->setEnabled(false);
    getChildByName<ui::Button*>(CLOSE_BUTTON_NAME)->setVisible(false);
    
    getChildByName<Label*>(UPGRADE_TITLE_NAME)->setString("LOADING");
    getChildByName<Label*>(UPGRADE_TITLE_NAME)->setPosition(STATUS_TEXT_POSITION);
    getChildByName<Label*>(UPGRADE_TITLE_NAME)->setColor(Color3B::GRAY);
}

void FullVersionLayer::onPurchase() {
    onBeginOperation();
    EconomicsManager::getInstance()->askPurchase([this](bool res, string error) {
        onGetResults(res, error);
    });
}

void FullVersionLayer::onRestore() {
    onBeginOperation();
    EconomicsManager::getInstance()->askRestore([this](bool res, string error) {
        onGetResults(res, error);
    });
}

void FullVersionLayer::onGetResults(bool res, string error) {
    _inProcess = false;
    getChildByName<ui::Button*>(CLOSE_BUTTON_NAME)->setEnabled(true);
    getChildByName<ui::Button*>(CLOSE_BUTTON_NAME)->setVisible(true);
    if(res) {
        getChildByName<Label*>(UPGRADE_TITLE_NAME)->setString("SUCCESS!");
        getChildByName<Label*>(UPGRADE_TITLE_NAME)->setPosition(STATUS_TEXT_POSITION);
	getChildByName<Label*>(UPGRADE_TITLE_NAME)->setColor(Color3B(51,122,10));
        EconomicsManager::getInstance()->setIsBought(true);
        ScenesManager::getInstance()->gotoMain();
	// TODO: play animation
    } else {
	getChildByName<Label*>(UPGRADE_TITLE_NAME)->setString("UPGRADE");
        getChildByName<Label*>(UPGRADE_TITLE_NAME)->setPosition(UPDATE_TEXT_POSITION);
	getChildByName<Label*>(UPGRADE_TITLE_NAME)->setColor(Color3B::WHITE);
	_buyButton->setEnabled(true);
	_buyButton->setVisible(true);
        _restoreButton->setEnabled(true);
        _restoreButton->setVisible(true);
        if(error == string("MarketPurchaseCancelled")) return;
        auto l = ErrorMessageBox::create();
        if(error == "NOTHING_TO_RESTORE") {
            l->setTitle("Restore error");
            l->setSubTitle("Looks like you haven't bought fullversion. \n Nothing to restore.");
        } else {
            l->setTitle("An error occured...");
            l->setSubTitle("Please check the internet connection \n and your store account");
        }
        addChild(l, 100);
        l->setOnOk([l, this]() {
            this->runAction(Sequence::create(
                DelayTime::create(0.1), CallFunc::create([l]() {
                    l->removeFromParent();
                }), NULL
            ));
        });
    }
}








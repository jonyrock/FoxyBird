#include "ResultsLayer.h"
#include "Managers/ScenesManager.h"
#include "Managers/LivesManager.h"
#include "Managers/ScoreManager.h"
#include "GUI/GUI.h"
#include "GUI/FullVersionLayer.h"
#include "Utils.h"
#include "Managers/SoundsManager.h"
#include <cocos/ui/UIButton.h>
#include <string>
#include <cocos/ui/UIScale9Sprite.h>


using namespace cocos2d;
using namespace std;


bool ResultsLayer::init() {
    if(!Layer::init()) return false;
    _size = Director::getInstance()->getVisibleSize();
    _isFullVersion = LivesManager::getInstance()->isUnlimited();
    initBackground();
    initLogo(); // it is also depends on _isFullVersion
    initPlayButton();
    initOverlayListener();
    if(_isFullVersion) {
        initFullVersionLogo();
    } else {
        initLives();
        initGetUnlimited();
        scheduleUpdate();
    }
    return true;
}

void ResultsLayer::initBackground() {
    auto background = LayerColor::create(Color4B(0,0,0,255/2));
    background->setOpacity(0);
    background->runAction(FadeTo::create(0.7f, (GLbyte)204));
    addChild(background);
}

void ResultsLayer::initLogo() {
    Sprite* sprite = nullptr;
    if(!_isFullVersion) 
        sprite = Sprite::create("full_version_fox.png");
    else 
        sprite = Sprite::create("full_version_fox_full.png");
    sprite->setAnchorPoint({0.5, 1});
    addChild(sprite, 2);
    Utils::setNodeWidthR(sprite, 160/1136.0);
    sprite->setPosition(Utils::coordFromPSD({1136/2,192}));
}

void ResultsLayer::initLives() {
    ScenesManager::getInstance()->showLives();
}

void ResultsLayer::initPlayButton() {
    auto button = ui::Button::create("play-again.png", "play-again-active.png", "play-again-active.png");
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
    button->setEnabled(false);
    //float fromScale = button->getScale() * 0.3;
    //float toScale = button->getScale();
    //button->setScale(fromScale);
    auto fromPos = Utils::coordFromPSD({1136/2, -1});
    auto toPos = button->getPosition();
    button->setPosition(fromPos);
    button->runAction(Sequence::create(
        EaseBounceOut::create(
            //ScaleTo::create(0.5, toScale)
            MoveTo::create(0.5, toPos)
        ),
        CallFunc::create([button]() { 
            button->setEnabled(true); 
        }), NULL
    ));
}

string spaceInserted(const string& str) {
    string res;
    unsigned int offset = str.size() % 3;
    for(size_t i = 0; i < str.size(); i++) {
	if((i % 3) == offset) res.push_back(' ');
	res.push_back(str.at(i));
    }
    return res;
}

void ResultsLayer::setScore(int score) {
    
    const float HORIZONTAL_OFFSET = 315;
    
    // VALUES
    bool isBest = ScoreManager::getInstance()->sendMyScore(score);
    int	 best	= ScoreManager::getInstance()->getBestScore();
    auto l1 = GUI::createLabelBI(spaceInserted(Utils::toString(score)), 52, TextHAlignment::RIGHT);
    l1->setColor(Color3B::WHITE);
    l1->setPosition(Utils::coordFromPSD({HORIZONTAL_OFFSET, 280}));
    addChild(l1, 2);
    auto l2 = GUI::createLabelBI(spaceInserted(Utils::toString(best)), 52, TextHAlignment::RIGHT);
    l2->setColor(Color3B::WHITE);
    l2->setPosition(Utils::coordFromPSD({-HORIZONTAL_OFFSET, 280}));
    addChild(l2, 2);
    
    // TITLES
    float hOffset = 25;
    if(isBest) {
	auto t1 = GUI::createLabelB("NEW \n BEST SCORE!", 25, TextHAlignment::CENTER);
	t1->setColor(Color3B(Utils::colorFromString("#00ffea")));
	t1->setPosition(Utils::coordFromPSD({HORIZONTAL_OFFSET,175+hOffset}));
	addChild(t1, 2);
	SoundsManager::getInstance()->playNewRecord();
    } else {
	auto t1 = GUI::createLabelB("SCORE", 25);
	t1->setColor(Color3B(255,198,1));
	t1->setPosition(Utils::coordFromPSD({HORIZONTAL_OFFSET,198+hOffset}));
	addChild(t1, 2);
	SoundsManager::getInstance()->playGameOverEffect();
    }
    auto t2 = GUI::createLabelB("BEST SCORE", 25);
    t2->setColor(Color3B(255,0,221));
    t2->setPosition(Utils::coordFromPSD({-HORIZONTAL_OFFSET,198+hOffset}));
    addChild(t2, 2);
}

void ResultsLayer::onBack() {
  if(ScenesManager::getInstance()->isFullVersionOpen()) {
    if(ScenesManager::getInstance()->isFullVersionInProcess()) 
      return;
    ScenesManager::getInstance()->hideFullVersion();
  } else {
    ScenesManager::getInstance()->toggleExitMessage();
  }
}

void ResultsLayer::initGetUnlimited() {
    
    auto node = Node::create();
    auto b = ui::Button::create("full_version_unlimited_off.png", "full_version_unlimited_on.png", "full_version_umlimited_on.png");
    auto l = GUI::createLabelB("GET UNLIMITED LIVES", 21);
    auto s = Sprite::create("ic_unlifes.png");
    Utils::setNodeWidthR(b, 387.0f/1136);
    Utils::setNodeWidthR(l, 296.0f/1136);
    Utils::setNodeWidthR(s, 50.f/1136);
    const float TARGET_S_SCALE = s->getScale();
    
    b->setAnchorPoint({0.185, 0.5});
    b->addTouchEventListener([this](Ref*, ui::Widget::TouchEventType touchType) {
	if(touchType == ui::Widget::TouchEventType::ENDED) {
	    SoundsManager::getInstance()->playButtonEffect();
	    onShowFull();
	}
    });
    
    // ANIMATION
    auto animHolder = Sprite::create("star.png");
    animHolder->runAction(RepeatForever::create(RotateBy::create(10, 360)));
    animHolder->runAction(RepeatForever::create(
	Sequence::create(ScaleTo::create(1, 0), ScaleTo::create(1, TARGET_S_SCALE), nullptr)
    ));
    animHolder->setScale(TARGET_S_SCALE);
    node->addChild(b);
    node->setContentSize(b->getContentSize());
    node->addChild(animHolder);
    s->setPosition({-s->getContentSize().width * 0.65f * s->getScale(), 0});
    l->setAnchorPoint({0, 0.5f});
    node->addChild(l);
    node->addChild(s);
    auto rrrr = Utils::coordFromPSD({685, 58+23/2});
    node->setPosition(rrrr);
    //node->setAnchorPoint({1,0});
    addChild(node);
    animHolder->setPosition(s->getPosition());
    s->setScale(TARGET_S_SCALE);
}

void ResultsLayer::onShowFull() {
    ScenesManager::getInstance()->showFullVersion();
}

void ResultsLayer::initOverlayListener() {
    auto l = EventListenerTouchOneByOne::create();
    l->onTouchBegan = [this](Touch*, Event*) {
	return true;
    };
    l->setSwallowTouches(true);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(l, this);
}

void ResultsLayer::initFullVersionLogo() {
    auto l = Sprite::create("fullversion_logo.png");
    l->setPosition(Utils::coordFromPSD({1136/2, 70}));
    Utils::setNodeWidthR(l, 400.0f/1136);
    addChild(l, 3);
}


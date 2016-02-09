#include "MessagesLayer.h"
#include "GUI.h"
#include "Utils.h"

using namespace cocos2d;
using namespace Utils;
using namespace std;

const int    SUCCESS_MESSAGES_SIZE = 6;
const string SUCCESS_TITLES[]      = { "Nice!"  , "Well Done!", "Amazing!", "Jumping spree!", "Unbelievable!", "Perfect!" };
const string SUCCESS_COLORS[]      = { "#39ffc5", "#39f3ff"   , "#ff8808" , "#fff000"       , "#fd4474"      , "#ff47af"  };

bool MessagesLayer::init() {
    if(!Node::init()) return false;
    _size = Director::getInstance()->getVisibleSize();
    initLabel();
    initScore();
    return true;
}

void MessagesLayer::initLabel() {
    _label = GUI::createLabelB("", 24, TextHAlignment::CENTER);
    _label->setColor(Color3B::WHITE);
    _label->setPosition(coordFromPSD({1136/2, 94+34/2}));
    addChild(_label, 2);
}

void MessagesLayer::initScore() {
    _scoreLabel = createLabel("0", Color3B::WHITE, 45);
    _scoreMultLabel = createLabel("0", Color3B::WHITE, 34);
    _scoreLabel->setPosition(coordFromPSD({1136/2, 28 + 45/2}));
    _scoreMultLabel->setPosition(coordFromPSD({1136/2, 94 + 34/2}));
    addChild(_scoreLabel);
    addChild(_scoreMultLabel);
    _scoreMultLabel->setVisible(false);
}

void MessagesLayer::setScore(int score) {
    _scoreLabel->setString(Utils::toString(score));
}

void MessagesLayer::setScoreMultNone() {
    _scoreMultLabel->setVisible(false);
}

void MessagesLayer::showBeginningMessage() {
    _beginningMessage1 = GUI::createLabelB("TAP TO START", 40);
    _beginningMessage1->setColor(Color3B::WHITE);
    _beginningMessage1->setAnchorPoint(Vec2(0.5, 0.6));
    _beginningMessage1->enableOutline(Color4B(0,0,0,110), Utils::getFontSize(3));
    _beginningMessage1->setPosition(Vec2(_size/2) + Vec2(0, _size.height * 0.01));
//    
//    _beginningMessage2 = GUI::createLabelR("A green arrow will appear. Click on it! \n It shows how to play.", 28, TextHAlignment::CENTER);
//    _beginningMessage2->setColor(Color3B(100,173,21));
//    _beginningMessage2->setAnchorPoint(Vec2(0.5, 0.6));
//    _beginningMessage2->enableOutline(Color4B(0,0,0,100), Utils::getFontSize(1.5));
//    _beginningMessage2->setPosition(Vec2(_size/2) - Vec2(0, _size.height * 0.05));
//    if(_beginningMessage2->getContentSize().width > _size.width * 0.9) {
//	Utils::setNodeWidthR(_beginningMessage2, 0.9);
//    }
    
    // TODO: again, you are the fox. Remember how the bird jumps
    // TODO: green arrows will show you how to play
    addChild(_beginningMessage1);
//    addChild(_beginningMessage2);
    
}

void MessagesLayer::hideBeginningMessage() {
    if(_beginningMessage1 != nullptr) _beginningMessage1->removeFromParent();
    if(_beginningMessage2 != nullptr) _beginningMessage2->removeFromParent();
}

void MessagesLayer::showLevel(int num) {
    // TODO: delete label in the end
    auto label = createLabel(string("Level ") + Utils::toString(num), Color3B::WHITE, 39);
    // animation from https://trello-attachments.s3.amazonaws.com/548e7fee1cf74f90683051f2/1158x935/fcdb8fab01a59ec8363561f9ff899976/levels.png
    
    // DURATIONS
    float d1 = 0.3;
    float d2 = 0.1;
    float d3 = 0.7;
    float d4 = 0.25;
    
    // POSITION
    label->setPosition(coordFromPSD({1136/2, 135 + 200}));
    label->runAction(Sequence::create(
        MoveTo::create(d1, coordFromPSD({1136/2, 125 + 200})),
        MoveTo::create(d2, coordFromPSD({1136/2, 130 + 200})),
        DelayTime::create(d3),
        MoveTo::create(d4, coordFromPSD({1136/2, 70 + 200})),
        nullptr
    ));
    
    // OPACITY
    label->setOpacity(0);
    label->runAction(Sequence::create(
         FadeTo::create(d1, 255),
         DelayTime::create(d2 + d3),
         FadeTo::create(d4, 0),
         nullptr
    ));
    
    if(_scoreMultLabel->isVisible()) {
        
    }
    
    addChild(label);

}

void MessagesLayer::showSuccessMessage(int i) {
    // TODO: delete label in the end
    if(i > SUCCESS_MESSAGES_SIZE) return;
    i--;
    Node* sprite = createLabel(
	SUCCESS_TITLES[i], Utils::colorFromString(SUCCESS_COLORS[i]), 41
    );
    
    // DURATIONS
    float durationIn = 1;
    float durationMiddle = 0.8;
    float durationOut = 0.2;
    
    _scoreMultLabel->setOpacity(0);
    _scoreMultLabel->runAction(Sequence::create(
	(_scoreMultLabel->isVisible() ? 
	    static_cast<ActionInterval*>(FadeOut::create(durationIn)): 
	    static_cast<ActionInterval*>(DelayTime::create(durationMiddle)) 
	),
	DelayTime::create(durationMiddle),
	CallFunc::create([this, i]() { 
	    _scoreMultLabel->setVisible(true);
	    _scoreMultLabel->setString(string("x") + Utils::toString(i+1));
	}),
	FadeIn::create(durationOut * 2),
	nullptr
    ));
    
    sprite->runAction(Sequence::create(
	DelayTime::create(durationIn + durationMiddle + durationOut),
	CallFunc::create([sprite]() { 
	    sprite->stopAllActions();
	    sprite->removeFromParent();
	}),
	nullptr
    ));
    
    // ROTATION
    sprite->runAction(Sequence::create(
	EaseOut::create(RotateBy::create(durationIn * 0.25, -4), 2), 
	EaseIn::create(RotateTo::create(durationIn * 0.25, 0), 2), 
	nullptr
    ));
    
    // OPACITY
    sprite->setOpacity(0);
    sprite->runAction(Sequence::create(
	FadeTo::create(durationIn * 0.5, 255), 
	DelayTime::create(durationIn * 0.5 + durationMiddle),
	FadeTo::create(durationOut, 0), 
	nullptr
    ));
    
    // POSITION
    //auto diff1 = coordFromPSD({560, 185}) - coordFromPSD({560, 125});
    //auto diff2 = coordFromPSD({560, 85}) - coordFromPSD({560, 125});
    sprite->setPosition(coordFromPSD({1136/2, 185}));
    sprite->runAction(Sequence::create(
	MoveTo::create(durationIn * 0.5, coordFromPSD({1136/2, 125})),
	DelayTime::create(durationIn * 0.5 + durationMiddle),
	MoveTo::create(durationOut, coordFromPSD({1136/2, 85})),
	nullptr
    ));
    
    // SCALE
    sprite->setScale(0.9);
    sprite->runAction(Sequence::create(
	EaseOut::create(ScaleTo::create(durationIn * 0.5, 1.2), 2), 
	EaseOut::create(ScaleTo::create(durationIn * 0.3, 0.9), 2), 
	EaseBackInOut::create(ScaleTo::create(durationIn * 0.2, 1)), 
	nullptr
    ));
    
    addChild(sprite);
    
}

Label* MessagesLayer::createLabel(const string& text, Color3B color, float s) {
    auto l1 = GUI::createLabelBI(text, s);
    l1->setColor(color);
    l1->setAnchorPoint(Vec2(0.5, 0.6));
    l1->enableOutline(Color4B(0,0,0,51), Utils::getFontSize(3));
    return l1;
}

void MessagesLayer::showHint() {
    
    // TODO: delete label in the end
    auto label = createLabel("Recollect where the bird was", Color3B::WHITE, 30);
    Utils::setNodeWidthR(label, 0.5);
    // animation from https://trello-attachments.s3.amazonaws.com/548e7fee1cf74f90683051f2/1158x935/fcdb8fab01a59ec8363561f9ff899976/levels.png
    
    // DURATIONS
    float d1 = 0.3;
    float d2 = 0.1;
    float d3 = 0.7;
    float d4 = 0.25;
    
    // POSITION
    label->setPosition(coordFromPSD({1136/2, 135 + 200}));
    label->runAction(Sequence::create(
        MoveTo::create(d1, coordFromPSD({1136/2, 125 + 200})),
        MoveTo::create(d2, coordFromPSD({1136/2, 130 + 200})),
        DelayTime::create(d3),
        MoveTo::create(d4, coordFromPSD({1136/2, 70 + 200})),
        nullptr
    ));
    
    // OPACITY
    label->setOpacity(0);
    label->runAction(Sequence::create(
         FadeTo::create(d1, 255),
         DelayTime::create(d2 + d3),
         FadeTo::create(d4, 0),
         nullptr
    ));
    
    if(_scoreMultLabel->isVisible()) {
        
    }
    
    addChild(label);
    
}













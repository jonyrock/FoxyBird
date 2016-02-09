#include "PauseMessageBox.h"
#include "Utils.h"
#include <cocos2d.h>

using namespace cocos2d;
using namespace std;

bool PauseMessageBox::init() {
    LayerColor::initWithColor(Color4B(0,0,0,200));
    _size = Director::getInstance()->getVisibleSize();
    initGUI();
    return true;
}

void PauseMessageBox::initGUI() {
    auto t = GUI::createLabelB("PAUSE", 69);
    t->setColor(Color3B::WHITE);
    t->setAnchorPoint({0.5,0.8});
    t->setPosition(Utils::coordFromPSD({1136/2, 200}));
    addChild(t);
    
    auto bc = GUI::TextButton::create({300,80}, "Continue", [this]() {
	if(_onContinue) _onContinue();
    });
    bc->setPosition(Utils::coordFromPSD({1136/2, -200}));
    addChild(bc, 2);
    
    // TODO: add to main button
//    auto bm = GUI::TextButton::create({200,80}, "Menu", [this]() {
//	if(_onContinue) _onContinue();
//    });
//    bm->setPosition(Utils::coordFromPSD({1136/2 - 100, -200}));
//    addChild(bm, 2);
    
}

void PauseMessageBox::setOnContinue(function<void()> fun) {
    _onContinue = fun;
}









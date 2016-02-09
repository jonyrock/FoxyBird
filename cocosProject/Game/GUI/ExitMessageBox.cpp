#include "ExitMessageBox.h"
#include "Utils.h"
#include <cocos2d.h>


using namespace cocos2d;
using namespace std;


bool ExitMessageBox::init() {
    LayerColor::initWithColor(Color4B(0,0,0,200));
    _size = Director::getInstance()->getVisibleSize();
    initGUI();
    return true;
}

void ExitMessageBox::initGUI() {
    auto t = GUI::createLabelB("Are you sure you \n want to exit?", 60, TextHAlignment::CENTER);
    t->setColor(Color3B::WHITE);
    t->setAnchorPoint({0.5,0.8});
    t->setPosition(Utils::coordFromPSD({1136/2, 160}));
    addChild(t);
    
    auto byes = GUI::TextButton::create({200,80}, "Yes", [this]() {
	Utils::exit();
    });
    
    auto bno = GUI::TextButton::create({200,80}, "No", [this]() {
	runAction(Sequence::create(
            DelayTime::create(0.2),
            CallFunc::create([this]() {
                close();
            }),
            nullptr
        ));
    });
    
    byes->setPosition(Utils::coordFromPSD({1136/2 - 210, -200}));
    bno->setPosition(Utils::coordFromPSD({1136/2 + 210, -200}));
    addChild(byes, 2);
    addChild(bno, 2);

}

void ExitMessageBox::close() {
  if(_onNo) _onNo();
  this->removeFromParent(); 
}

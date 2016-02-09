#include "ErrorMessageBox.h"
#include "GUI/GUI.h"
#include "Utils.h"
#include <cocos2d.h>


#define TITLE_NAME "titleName"
#define SUB_TITLE_NAME "subTitleName"


using namespace cocos2d;
using namespace std;


bool ErrorMessageBox::init() {
    LayerColor::initWithColor(Color4B(10,0,0,240));
    _size = Director::getInstance()->getVisibleSize();
    initGUI();
    return true;
}

void ErrorMessageBox::setTitle(const string& txt) {
    auto t = getChildByName<Label*>(TITLE_NAME);
    t->setString(txt);
}

void ErrorMessageBox::setSubTitle(const string& txt) {
    auto t = getChildByName<Label*>(SUB_TITLE_NAME);
    t->setString(txt);
}

void ErrorMessageBox::initGUI() {
    auto t = GUI::createLabelB("==NO TEXT==", 49);
    t->setColor(Color3B::WHITE);
    t->setAnchorPoint({0.5,0.8});
    t->setPosition(Utils::coordFromPSD({1136/2, 140}));
    addChild(t, 2, TITLE_NAME);
    
    auto st = GUI::createLabelR("==NO TEXT==", 32, TextHAlignment::CENTER);
    st->setColor(Color3B::WHITE);
    st->setAnchorPoint({0.5,0.8});
    st->setPosition(Utils::coordFromPSD({1136/2, 240}));
    addChild(st, 2, SUB_TITLE_NAME);
     
    auto b = GUI::TextButton::create({300,80}, "Ok", [this]() {
	if(_onOkCallback) _onOkCallback();
    });
    b->setPosition(Utils::coordFromPSD({1136/2, -150}));
    addChild(b, 2);
}

void ErrorMessageBox::setOnOk(function<void()> fun) {
    _onOkCallback = fun;
}









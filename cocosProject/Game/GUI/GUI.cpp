#include <ui/UIScale9Sprite.h>

#include "GUI/GUI.h"
#include "Managers/SettingsManager.h"
#include "Managers/SoundsManager.h"
#include "Utils.h"


using namespace cocos2d;
using namespace std;

/* SWAP BUTTON */
GUI::SwapButton* GUI::SwapButton::create(const string& icon1, const string& icon2) {
    auto *pRet = new SwapButton(); 
    if (pRet && pRet->init(icon1, icon2)) {
	pRet->autorelease();
	return pRet;
    } else { 
	delete pRet;
	pRet = NULL;
	return NULL;
    } 
}

bool GUI::SwapButton::init(const string& icon1, const string& icon2) {
    if(!Node::init()) return false;
    _buttonOn = ui::Button::create(icon1, icon1, icon1);
    _buttonOff = ui::Button::create(icon2, icon2, icon2);
    addChild(_buttonOn);
    addChild(_buttonOff);
    _buttonOn->addTouchEventListener([this](Ref*, ui::Widget::TouchEventType touchType) {
	if(touchType == ui::Widget::TouchEventType::ENDED) {
	    SoundsManager::getInstance()->playButtonEffect();
	    setSwitched(false);
	    if(_onChangeValue) _onChangeValue(false);
	}
    });
    _buttonOff->addTouchEventListener([this](Ref*, ui::Widget::TouchEventType touchType) {
	if(touchType == ui::Widget::TouchEventType::ENDED) {
	    SoundsManager::getInstance()->playButtonEffect();
	    setSwitched(true);
	    if(_onChangeValue) _onChangeValue(true);
	}
    });
    setSwitched(true);
    setContentSize(_buttonOn->getContentSize());
    return true;
}

void GUI::SwapButton::setSwitched(bool enabled) {
    _buttonOn->setVisible(enabled);
    _buttonOff->setVisible(!enabled);
}

bool GUI::SwapButton::isEnabled() const {
    return _buttonOn->isVisible();
}

void GUI::SwapButton::setOnChange(const function<void(bool)>& fun) {
    _onChangeValue = fun;
}

void GUI::SwapButton::triggerChange() {
    SoundsManager::getInstance()->playButtonEffect();
    setSwitched(!isEnabled());
    if(_onChangeValue) _onChangeValue(false);
}

/* TEXT BUTTON */
GUI::TextButton* GUI::TextButton::create(
    const Size& size, const string& text, function<void()> callback,
    cocos2d::Color3B c1, cocos2d::Color3B c2, cocos2d::Color3B c3, cocos2d::Color3B c4,float textSize
) {
    auto widget = new TextButton();
    if (widget && widget->init(size, text, callback,c1,c2,c3,c4,textSize)) {
        widget->autorelease();
        return widget;
    }
    CC_SAFE_DELETE(widget);
    return nullptr;
}

bool GUI::TextButton::init(
    const Size& size, const string& text, 
    function<void()> callback,
    cocos2d::Color3B c1, cocos2d::Color3B c2, cocos2d::Color3B c3, cocos2d::Color3B c4,
    float textSize
) {
    if(!ui::Button::init("btn.png", "btn_active.png"))
        return false;
    this->c3 = c3;
    this->c4 = c4;
    _buttonNormalRenderer->setColor(c1);
    _buttonClickedRenderer->setColor(c2);
    setScale9Enabled(true);
    setContentSize(size);
    _onClickCallback = callback;
    // text
    _text = GUI::createLabelB(text, textSize);
    _text->setColor(c3);
    _text->setPosition(size/2);
    addChild(_text);
    _inited = true;
    return true;
}

void GUI::TextButton::onPressStateChangedToNormal() {
    ui::Button::onPressStateChangedToNormal();
    if(!_inited) return;
    SoundsManager::getInstance()->playButtonEffect();
    _text->setColor(c3);
    _onClickCallback();
}

void GUI::TextButton::onPressStateChangedToPressed() {
    ui::Button::onPressStateChangedToPressed();
    _text->setColor(c4);
}


/* LABELS */

Label* GUI::createLabelR(const string& text, float size, TextHAlignment hAlign) {
    size = Utils::getFontSize(size);
    return Label::createWithTTF(
	text, "fonts/Scada-Regular.ttf", size, Size::ZERO, hAlign
    );
}

Label* GUI::createLabelB(const string& text, float size, TextHAlignment hAlign) {
    size = Utils::getFontSize(size);
    return Label::createWithTTF(
	text, "fonts/Scada-Bold.ttf", size, Size::ZERO, hAlign
    );
}

Label* GUI::createLabelI(const string& text, float size, TextHAlignment hAlign) {
    size = Utils::getFontSize(size);
    return Label::createWithTTF(
	text, "fonts/Scada-Italic.ttf", size, Size::ZERO, hAlign
    );
}

Label* GUI::createLabelBI(const string& text, float size, TextHAlignment hAlign) {
    size = Utils::getFontSize(size);
    return Label::createWithTTF(
	text, "fonts/Scada-BoldItalic.ttf", size, Size::ZERO, hAlign
    );
}

#include "SettingsAsset.h"
#include "Managers/SettingsManager.h"
#include "Managers/SoundsManager.h"
#include "Utils.h"
#include <string>
#include <functional>
#include <cocos/ui/UIButton.h>
#include "GUI.h"


using namespace cocos2d;
using namespace std;


bool SettingsAsset::init() {
    if(!Node::init()) return false;
    auto s = Director::getInstance()->getVisibleSize();
    auto rr = (s.width / s.height) / (1136.0f/640);
    _size = Size(100, s.height * 0.5 * rr);
    initBack();
    initIcons();
    return true;
}

void SettingsAsset::initBack() {
    _back = LayerColor::create(Color4B(100, 100, 100, 0), _size.width, _size.height);
    _back->setPosition({
	-_size.width / 2, 
	-_size.height * (2 * ELEMENTS_NUMBER - 1) / (2 * ELEMENTS_NUMBER)
    });
    _back->setVisible(false);
    addChild(_back, 1);
}

void SettingsAsset::initIcons() {
    auto mainButton = ui::Button::create("settings_settings.png", "settings_settings.png", "settings_settings.png");
    Utils::setNodeWidthR(mainButton, 82.0f/1136);
    addChild(mainButton, 2);
    mainButton->addTouchEventListener([this](Ref*, ui::Widget::TouchEventType touchType) {
        if(touchType == ui::Widget::TouchEventType::ENDED) {
	    SoundsManager::getInstance()->playButtonEffect();
            _back->setVisible(!_back->isVisible());
        }
    });
    auto music   = GUI::SwapButton::create("settings_music_on.png", "settings_music_off.png");
    auto effects = GUI::SwapButton::create("settings_effects_on.png", "settings_effects_off.png");
    Utils::setNodeWidthR(music, 82.0f/1136);
    Utils::setNodeWidthR(effects, 82.0f/1136);
    music  ->setPosition({_size.width/2, _size.height - 1.5f * _size.height/ELEMENTS_NUMBER});
    effects->setPosition({_size.width/2, _size.height - 2.5f * _size.height/ELEMENTS_NUMBER});
    _back->addChild(music);
    _back->addChild(effects);
    music  ->setSwitched(SettingsManager::getInstance()->getIsMusicOn());
    effects->setSwitched(SettingsManager::getInstance()->getIsSoundEffectsOn());
    music->setOnChange([](bool value) {
	SettingsManager::getInstance()->setIsMusicOn(value);
    });
    effects->setOnChange([](bool value) {
	SettingsManager::getInstance()->setIsSoundEffectsOn(value);
    });
}

void SettingsAsset::close() {
    _back->setVisible(false);
}

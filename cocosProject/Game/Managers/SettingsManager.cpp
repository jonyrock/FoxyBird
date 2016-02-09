#include "Managers/SettingsManager.h"
#include "Managers/DataManager.h"
#include "Managers/SoundsManager.h"
#include "Managers/DataManager.h"
#include "Managers/LocalizationManager.h"

#define DB_IS_MUSIC "isMusicOn"
#define DB_IS_EFFECTS "isEffectsOn"
#define DB_IS_TUTORIAL "isTutorialOn"
#define DB_LANGUAGE "language"

using namespace cocos2d;

SettingsManager* SettingsManager::getInstance() {
    static SettingsManager instance;
    return &instance;
}

bool SettingsManager::getIsMusicOn() {
    return DataManager::getInstance()->get(DB_IS_MUSIC, true);
}

void SettingsManager::setIsMusicOn(bool on) {
    DataManager::getInstance()->put(DB_IS_MUSIC, on);
    SoundsManager::getInstance()->onMusicSettingsChange(on);
}

bool SettingsManager::getIsSoundEffectsOn() {
    return DataManager::getInstance()->get(DB_IS_EFFECTS, true);
}

void SettingsManager::setIsSoundEffectsOn(bool on) {
    DataManager::getInstance()->put(DB_IS_EFFECTS, on);
}

bool SettingsManager::getIsTutorial() {
    return DataManager::getInstance()->get(DB_IS_TUTORIAL, true);
}

void SettingsManager::setIsTutorial(bool on) {
    DataManager::getInstance()->put(DB_IS_TUTORIAL, on);
}

LanguageType SettingsManager::getLanguage() {
    return (LanguageType) DataManager::getInstance()->get(
        DB_LANGUAGE, 
        (int)Application::getInstance()->getCurrentLanguage()
    );
}

void SettingsManager::setLanguage(LanguageType lang) {
    DataManager::getInstance()->put(DB_LANGUAGE, (int)lang);
    LocalizationManager::getInstance()->loadLanguage(lang);
}

Size SettingsManager::getWinSize() {
    auto size = Director::getInstance()->getOpenGLView()->getVisibleSize();
    return size;
}
#pragma once

#include "Managers/SettingsManager.h"

#include <cocos2d.h>

#include <string>
#include <vector>
#include <map>

// TODO: make inheriencs from BasicManager

class LocalizationManager {

public:
    static LocalizationManager* getInstance();
    std::string localize(const std::string& key);
    void setLanguage(cocos2d::LanguageType lang);
    std::vector<cocos2d::LanguageType> getAvailableLanguages();
    cocos2d::Sprite* getSpriteForLanguage(cocos2d::LanguageType lang);
    std::string getNameForLanguage(cocos2d::LanguageType lang);

private:
    friend SettingsManager;
    LocalizationManager();
    void loadLanguage(cocos2d::LanguageType lang);
    cocos2d::LanguageType _currentLang;
    std::map<std::string, std::string> _localizedStrings;
    
};

inline std::string loc(const std::string& str) {
    return LocalizationManager::getInstance()->localize(str);
}
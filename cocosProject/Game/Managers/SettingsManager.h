#pragma once

#include "Managers/BasicManager.h"

#include <cocos2d.h>

class SettingsManager : public BasicManager {
public:
    
    static SettingsManager* getInstance();
    
    virtual inline bool init() { return true; }

    bool getIsMusicOn();
    void setIsMusicOn(bool);
    
    bool getIsSoundEffectsOn();
    void setIsSoundEffectsOn(bool);
    
    bool getIsTutorial();
    void setIsTutorial(bool);
    
    cocos2d::LanguageType getLanguage();
    void setLanguage(cocos2d::LanguageType lang);
    
    cocos2d::Size getWinSize();
    
    virtual inline ~SettingsManager() { }
    
private:
    SettingsManager() {}
    
};
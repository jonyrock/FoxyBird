#include "Managers/AdvertisingManager.h"
#include "Managers/EconomicsManager.h"
#include "Managers/DataManager.h"

#include "SonarFrameworks.h"

#include <cocos2d.h>


#define DB_TRIGGER_COUNTER "triggerCounter"


using namespace cocos2d;


AdvertisingManager* AdvertisingManager::getInstance() {
    static AdvertisingManager instance;
    return &instance;
}

bool AdvertisingManager::init() {
    _triggerCounter = DataManager::getInstance()->get(DB_TRIGGER_COUNTER, 1);
    SonarCocosHelper::Chartboost::preloadFullscreenAd();
    return true;
}

void AdvertisingManager::triggerAdv() {
    if(EconomicsManager::getInstance()->isBought()) {
        return;
    }

    if(_triggerCounter <= 2) {
        _triggerCounter++;
        DataManager::getInstance()->put(DB_TRIGGER_COUNTER, _triggerCounter);
        return;
    }
    
    bool ready = SonarCocosHelper::Chartboost::isFullscreenAdReady();
    if(ready) {
        showFullAd();
        _triggerCounter = 1;
        DataManager::getInstance()->put(DB_TRIGGER_COUNTER, _triggerCounter);
    }

    SonarCocosHelper::Chartboost::preloadFullscreenAd();
}

void AdvertisingManager::showFullAd() {
    SonarCocosHelper::Chartboost::showFullscreenAd();
}
void AdvertisingManager::hideFullAd() {

}


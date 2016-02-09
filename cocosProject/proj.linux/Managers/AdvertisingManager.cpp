#include "Managers/AdvertisingManager.h"

#include <cocos2d.h>

using namespace cocos2d;

class AdvertisingManagerImpl : public AdvertisingManager {

    virtual bool init() override {
	return true;
    }
    
    virtual bool isFullAdReady() override {
	return true;
    }
    

    virtual void hideFullAd() override {
	
    }
    
    virtual void showFullAd() override {
	
    }

};

AdvertisingManager* AdvertisingManager::getInstance() {
    static AdvertisingManagerImpl inst;
    return &inst;
}


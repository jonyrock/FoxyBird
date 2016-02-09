#pragma once

#include "Managers/DataManager.h"

class AdvertisingManagerJni {

public:
    static AdvertisingManagerJni* getInstance() {
	static AdvertisingManagerJni instance;
	return &instance;
    }

    
    inline int setBottomHeight(int height) {
	_bottomHeight = height;
	DataManager::getInstance()->put("android_ad_height_cache", height);
    }
    
    static const int UNDEFINED_HEIGHT = -1;
    
private:
    int _bottomHeight = UNDEFINED_HEIGHT;
    bool _wasDbQuery = false;
    
};
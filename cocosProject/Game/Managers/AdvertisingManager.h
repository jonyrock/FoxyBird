#pragma once

#include "BasicManager.h"


class AdvertisingManager : public BasicManager {
public:
    static AdvertisingManager* getInstance();
    
    virtual bool init();
    
    void triggerAdv();

    virtual void hideFullAd();

protected:

    virtual void showFullAd();
    
private:
    int _triggerCounter;
    
};
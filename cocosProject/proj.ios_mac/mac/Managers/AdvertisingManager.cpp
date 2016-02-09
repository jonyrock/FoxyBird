#include "Managers/AdvertisingManager.h"




AdvertisingManager* AdvertisingManager::getInstance() {
    static AdvertisingManager instance;
    return &instance;
}

bool AdvertisingManager::init() {
    return true;
}

void AdvertisingManager::triggerAdv() {
}

void AdvertisingManager::showFullAd() {    
}
void AdvertisingManager::hideFullAd() {

}


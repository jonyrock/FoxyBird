#include "ReviewManager.h"
#include "Managers/DataManager.h"
#include "Managers/AnalyticsManager.h"

#define DB_REVIEW "isReviewOn"


ReviewManager* ReviewManager::getInstance() {
    static ReviewManager insntance;
    return &insntance;
}

bool ReviewManager::init() {
    _reviewCount = 0;
    _isTurnedOn = DataManager::getInstance()->get(DB_REVIEW, true);
    return true;
}

void ReviewManager::triggerReview() {

    showReview();
//    _reviewCount++;
//    if(_reviewCount > 1) {
//        
//    }
}

void ReviewManager::onYes() {
    AnalyticsManager::getInstance()->sendEvent("review: yes");
    DataManager::getInstance()->get(DB_REVIEW, false);
}

void ReviewManager::onLater() {
    AnalyticsManager::getInstance()->sendEvent("review: later");
}

void ReviewManager::onNo() {
    AnalyticsManager::getInstance()->sendEvent("review: no");
    DataManager::getInstance()->get(DB_REVIEW, false);
}
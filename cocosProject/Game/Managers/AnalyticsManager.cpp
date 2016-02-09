#include "AnalyticsManager.h"
#include "SonarFrameworks.h"

using namespace std;

AnalyticsManager* AnalyticsManager::getInstance() {
    static AnalyticsManager instance;
    return &instance;
}

bool AnalyticsManager::init() {
    return true;
}

void AnalyticsManager::setScene(const string& str) {
#if !CC_TARGET_OS_MAC
    SonarCocosHelper::GoogleAnalytics::setScreenName(str);
#endif
}

void AnalyticsManager::sendEvent(const string& str) {
#if !CC_TARGET_OS_MAC
    SonarCocosHelper::GoogleAnalytics::sendEvent(
        str, str, str, 0
    );
#endif
}
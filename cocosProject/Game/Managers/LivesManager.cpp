#include "LivesManager.h"
#include "DataManager.h"
#include "EconomicsManager.h"

#include "Utils.h"

#include "LocalNotification.h"

#include <cocos2d.h>

#include <algorithm>

#define DB_LIVES_AVAILABLE "lifesAvailable"
#define DB_TIME_TO_NEXT_LIFE "timePointToNextLife"
#define MAX_LIVES_NUMBER 8
#define LIFE_REFILL_DURATION chrono::seconds(60 * 2 + 30)
#define NOTIFICATION_ID 112712


using namespace std;
using namespace cocos2d;


LivesManager* LivesManager::getInstance() {
    static LivesManager instance;
    return &instance;
}

LivesManager::LivesManager() : _livesAvailable(MAX_LIVES_NUMBER) { }

bool LivesManager::init() {
    loadData();
    recount();
    return true;
}

void LivesManager::takeLife() {
    setLivesAvailable(_livesAvailable - 1);
}

void LivesManager::getStringTime(std::string& str) {
    auto now = chrono::system_clock::now();
    recount();
    auto duration = _timePointToNextLife - now;
    auto minutes = chrono::duration_cast<chrono::minutes>(duration);
    duration -= minutes;
    auto seconds = chrono::duration_cast<chrono::seconds>(duration);
    auto s = Utils::toString(minutes.count());
    if(s.length() < 2) {
        s = "0" + s;
    }
    str.append(s);
    str.append(":");
    s = Utils::toString(seconds.count());
    if(s.length() < 2) {
        s = "0" + s;
    }
    str.append(s);
    
}

int LivesManager::getLivesAvailable() {
    if(isUnlimited()) {
        return INT_MAX;
    }
    recount();
    return _livesAvailable;
}

int LivesManager::getLivesTookCurrentSession() const {
    return _livesTookCurrentSession;
}

void LivesManager::setTimePointToNextLife(chrono::time_point<chrono::system_clock> v) {
    _timePointToNextLife = v;
    DataManager::getInstance()->put(
        DB_TIME_TO_NEXT_LIFE, 
        (int)chrono::system_clock::to_time_t(_timePointToNextLife)
    );
}

void LivesManager::recount() {
    // TODO: remove this loop
    auto now = chrono::system_clock::now();
    while(true) {
        if(_livesAvailable >= MAX_LIVES_NUMBER) {
            break;
        }
        if(now < _timePointToNextLife) {
            break;
        }
        setTimePointToNextLife(_timePointToNextLife + LIFE_REFILL_DURATION);
        setLivesAvailable(_livesAvailable + 1);
    }
}

void LivesManager::loadData() {
    // this is the only case when we set _lifesAvailable manually
    _livesAvailable = DataManager::getInstance()->get(
        DB_LIVES_AVAILABLE, MAX_LIVES_NUMBER
    );
    _livesAvailable = min(_livesAvailable, MAX_LIVES_NUMBER);
    int ticks = DataManager::getInstance()->get(
        DB_TIME_TO_NEXT_LIFE,
        static_cast<int>(
            chrono::system_clock::to_time_t(chrono::system_clock::now())
        )
    );
    // this is the only case when we set _timePointToNextLife manually
    _timePointToNextLife = chrono::system_clock::from_time_t(ticks);
}

void LivesManager::setLivesAvailable(int v) {
    bool wasFull = isFull();
    _livesAvailable = v;
    if(wasFull && !isFull()) {
        setTimePointToNextLife(chrono::system_clock::now() + LIFE_REFILL_DURATION);
    }
    
    DataManager::getInstance()->put(
        DB_LIVES_AVAILABLE, _livesAvailable
    );
    // notification on full refill
    if(!isFull()) {
        auto timeSpan = _timePointToNextLife - chrono::system_clock::now();
        if(_livesAvailable + 1 < MAX_LIVES_NUMBER) {
            timeSpan += (MAX_LIVES_NUMBER - 1 - _livesAvailable) * LIFE_REFILL_DURATION;
        }
        int secondsToFull = chrono::duration_cast<chrono::seconds>(timeSpan).count();
#if !CC_TARGET_OS_MAC
        LocalNotification::cancel(NOTIFICATION_ID);
        LocalNotification::show(
            "Foxy's lives are full! Catch the bird!", 
            secondsToFull, NOTIFICATION_ID
        );
#endif
    }
}

bool LivesManager::isFull() {
    return _livesAvailable == MAX_LIVES_NUMBER;
}

bool LivesManager::isUnlimited() const {
    return EconomicsManager::getInstance()->isBought();
}

bool LivesManager::isEmpty() {
    return getLivesAvailable() == 0;
}

void LivesManager::askAboutNotificatoins() {
#if !CC_TARGET_OS_MAC
    LocalNotification::askAboutNotification();
#endif
}

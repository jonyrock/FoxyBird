/*
 Copyright (C) 2012-2014 Soomla Inc.
 
 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at
 
 http://www.apache.org/licenses/LICENSE-2.0
 
 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
 */

#include "CCSequenceReward.h"
#include "CCRewardStorage.h"

soomla::CCSequenceReward *soomla::CCSequenceReward::create(
        cocos2d::__String *id,
        cocos2d::__String *name,
        cocos2d::__Array *rewards
) {

    CCSequenceReward *ret = new CCSequenceReward();
    if (ret->init(id, name, rewards, NULL)) {
        ret->autorelease();
    }
    else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

soomla::CCSequenceReward::~CCSequenceReward() {
    CCLOG("ADADASDASSD");
}

const char *soomla::CCSequenceReward::getType() const {
    return CCCoreConsts::JSON_JSON_TYPE_SEQUENCE;
}

soomla::CCReward *soomla::CCSequenceReward::getLastGivenReward() {
    int idx = CCRewardStorage::getInstance()->getLastSeqIdxGiven(this);
    if (idx < 0) {
        return NULL;
    }
    return (CCReward *) getRewards()->getObjectAtIndex(idx);
}

bool soomla::CCSequenceReward::hasMoreToGive() {
    return CCRewardStorage::getInstance()->getLastSeqIdxGiven(this) < getRewards()->count();
}

bool soomla::CCSequenceReward::forceNextRewardToGive(soomla::CCReward *reward) {
    for (unsigned int i = 0; i < getRewards()->count(); i++) {
        if (reward->equals(getRewards()->getObjectAtIndex(i))) {
            CCRewardStorage::getInstance()->setLastSeqIdxGiven(this, i - 1);
            return true;
        }
    }
    return false;
}

bool soomla::CCSequenceReward::takeInner() {
    int idx = CCRewardStorage::getInstance()->getLastSeqIdxGiven(this);
    if (idx <= 0) {
        return false; // all rewards in the sequence were taken
    }
    CCRewardStorage::getInstance()->setLastSeqIdxGiven(this, (unsigned int) --idx);
    return true;
}

bool soomla::CCSequenceReward::giveInner() {
    int idx = CCRewardStorage::getInstance()->getLastSeqIdxGiven(this);
    CCLOG("%d", idx);
    if (idx >= getRewards()->count()) {
        return false; // all rewards in the sequence were given
    }
    CCRewardStorage::getInstance()->setLastSeqIdxGiven(this, (unsigned int) ++idx);
    return true;
}

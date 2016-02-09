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

#include "CCCoreBridge.h"
#include "CCDomainFactory.h"
#include "CCBadgeReward.h"
#include "CCRandomReward.h"
#include "CCSequenceReward.h"
#include "CCCoreEventDispatcher.h"
#include "CCSoomlaUtils.h"
#include "CCNativeCoreBridge.h"

using namespace cocos2d;

namespace soomla {

    USING_NS_CC;

#define TAG "SOOMLA CoreBridge"

    static CCCoreBridge *sInstance = NULL;

    CCCoreBridge *CCCoreBridge::getInstance() {
        if (!sInstance)
        {
            #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS) || (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
            sInstance = new CCNativeCoreBridge();
            #else
            sInstance = new CCCoreBridge();
            #endif
            sInstance->retain();
        }
        return sInstance;
    }

    void CCCoreBridge::initShared() {
        CCCoreBridge *coreBridge = CCCoreBridge::getInstance();
        if (!coreBridge->init()) {
            exit(1);
        }
    }

    CCCoreBridge::CCCoreBridge() {
    }

    bool CCCoreBridge::init() {

        CCCoreEventDispatcher::getInstance();    // to get sure it's inited

        CCDomainFactory::getInstance()->registerCreator(CCCoreConsts::JSON_JSON_TYPE_BADGE,
                &CCBadgeReward::createWithDictionary);
        CCDomainFactory::getInstance()->registerCreator(CCCoreConsts::JSON_JSON_TYPE_RANDOM,
                &CCRandomReward::createWithDictionary);
        CCDomainFactory::getInstance()->registerCreator(CCCoreConsts::JSON_JSON_TYPE_SEQUENCE,
                &CCSequenceReward::createWithDictionary);
        CCDomainFactory::getInstance()->registerCreator(CCCoreConsts::JSON_JSON_TYPE_SCHEDULE,
                &CCSchedule::createWithDictionary);
        CCDomainFactory::getInstance()->registerCreator(CCCoreConsts::JSON_JSON_TYPE_DATE_TIME_RANGE,
                &CCSchedule::CCDateTimeRange::createWithDictionary);

        return true;
    }
}

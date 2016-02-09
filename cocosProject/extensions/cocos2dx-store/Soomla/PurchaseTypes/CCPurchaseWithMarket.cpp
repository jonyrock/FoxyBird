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

#include "CCPurchaseWithMarket.h"
#include "CCSoomlaUtils.h"
#include "CCStoreEventDispatcher.h"
#include "CCSoomlaStore.h"
#include "CCStoreInfo.h"

namespace soomla {
    
    #define TAG "SOOMLA PurchaseWithMarket"
    
    CCPurchaseWithMarket *CCPurchaseWithMarket::create(cocos2d::__String *productId, cocos2d::__Double *price) {
        return createWithMarketItem(CCMarketItem::create(
                productId, price));
    }

    CCPurchaseWithMarket *CCPurchaseWithMarket::createWithMarketItem(CCMarketItem *marketItem) {
        CCPurchaseWithMarket *ret = new CCPurchaseWithMarket();
        if (ret->initWithMarketItem(marketItem)) {
            ret->autorelease();
        }
        else {
            CC_SAFE_DELETE(ret);
        }

        return ret;
    }

    bool CCPurchaseWithMarket::initWithMarketItem(CCMarketItem *marketItem) {
        bool res = CCPurchaseType::init();
        if (res) {
            setMarketItem(marketItem);
            return true;
        } else {
            return false;
        }
    }
    
    void CCPurchaseWithMarket::buy(const char* payload, CCError **error) {
        CCSoomlaUtils::logDebug(TAG, cocos2d::__String::createWithFormat("Starting in-app purchase for productId: %s",
                                                                         getMarketItem()->getProductId()->getCString())->getCString());
        
        if (payload == NULL) {
            payload = "";
        }
        
        CCPurchasableVirtualItem *pvi = dynamic_cast<CCPurchasableVirtualItem *>(CCStoreInfo::sharedStoreInfo()->getItemByItemId(getAssociatedItemId()->getCString(), error));
        CCStoreEventDispatcher::getInstance()->onItemPurchaseStarted(pvi, true);
        CCSoomlaStore::getInstance()->buyMarketItem(getMarketItem()->getProductId()->getCString(), payload, error);
    }

    CCPurchaseWithMarket::~CCPurchaseWithMarket() {
        CC_SAFE_RELEASE(mMarketItem);
    }
    
    bool CCPurchaseWithMarket::canAfford(CCError **error) {
        // for market purchases, always assume it can be afforded
        return true;
    }
}

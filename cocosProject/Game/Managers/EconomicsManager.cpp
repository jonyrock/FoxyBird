#include "EconomicsManager.h"

#include "Config.h"

#include "CCVirtualGood.h"
#include "CCPurchaseWithMarket.h"
#include "Cocos2dxStore.h"
#include "CCStoreAssets.h"

#include <cassert>


using namespace soomla;
using namespace cocos2d;
using namespace std;


const char* TAG        = "EconomicsManagerImpl";
const char* PRODUCT_ID = "fullVersionId";


struct FoxyBirdAssets : public soomla::CCStoreAssets {
    
    CREATE_FUNC(FoxyBirdAssets);
    
    __Array* _goods = nullptr;
    
    bool init() {
        auto noAdsLTVG = CCLifetimeVG::create(
            __String::create("Get full version"),
            __String::create("Get full version + no ads"),
            __String::create(PRODUCT_ID),
            CCPurchaseWithMarket::createWithMarketItem(
                CCMarketItem::create(
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
                    __String::create("noads_id"),
#endif 

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
                    __String::create("no_ads"),
#endif       
                    CCDouble::create(0.99)
                )
            )
        );
        _goods = CCArray::create(noAdsLTVG, NULL);
        _goods->retain();
        
        return true;
    }
    int getVersion() override { return 0; }
    __Array* getCurrencies() override { return NULL; }
    __Array* getGoods() override { return _goods; }
    __Array* getCurrencyPacks() override { return NULL; }
    __Array* getCategories() override { return NULL;}
    ~FoxyBirdAssets() { CC_SAFE_RELEASE(_goods); }
};

struct EconomicsManagerImpl : public EconomicsManager {
    
    function<void(bool, string)> _callbackPurchase = nullptr;
    function<void(bool, string)> _callbackRestore = nullptr;
    
    /* INITS */
    
    bool init() override {
        CCSoomla::initialize("customSecret");
        auto assets = FoxyBirdAssets::create();
        __Dictionary* storeParams = __Dictionary::create();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        storeParams->setObject(
            __String::create(Config::Android::PUBLIC_KEY), "androidPublicKey"
        );
#endif
        storeParams->setObject(__Bool::create(false), "testPurchases");
        storeParams->setObject(__String::create("xxxx"), "customSecret");
        CCSoomlaStore::initialize(assets, storeParams);
        initHandlers();
        return true;
    }
    
    void initHandlers() {
        CCLOG("EconomicsManager initHandlers");
        auto eventDispatcher = Director::getInstance()->getEventDispatcher();
        eventDispatcher->addCustomEventListener(
            CCStoreConsts::EVENT_MARKET_PURCHASE,
            CC_CALLBACK_1(EconomicsManagerImpl::onMarketPurchase, this)
        );
        eventDispatcher->addCustomEventListener(
            CCStoreConsts::EVENT_MARKET_PURCHASE_CANCELED,
            CC_CALLBACK_1(EconomicsManagerImpl::onMarketPurchaseCancelled, this)
        );
        eventDispatcher->addCustomEventListener(
            CCStoreConsts::EVENT_RESTORE_TRANSACTION_FINISHED,
            CC_CALLBACK_1(EconomicsManagerImpl::onRestoreTransactionsFinished, this)
        );
        eventDispatcher->addCustomEventListener(
            CCStoreConsts::EVENT_UNEXPECTED_ERROR_IN_STORE,
            CC_CALLBACK_1(EconomicsManagerImpl::onUnexpectedErrorInStore, this)
        );
    }
    
    /* INTERFACE */
    void askPurchase(function<void(bool, string)> callback) override {
        if(_callbackPurchase != nullptr || _callbackRestore != nullptr) {
            callback(false, "Manager is processing another operation");
            return;
        }
        _callbackPurchase = callback;
        CCError* soomlaError = NULL;
        
        CCStoreInventory::sharedStoreInventory()->buyItem(PRODUCT_ID, &soomlaError);
        if(soomlaError) {
            callCallbackPurchase(false, "Buy error");
            CCSoomlaUtils::logException("EconomicsManager::askPurchase", soomlaError);
        }
    }
    
    void askRestore(function<void(bool, string)> callback) override {
        if(_callbackRestore != nullptr || _callbackRestore != nullptr) {
            callback(false, "Manager is processing other operation");
            return;
        }
        _callbackRestore = callback;
        CCSoomlaStore::getInstance()->restoreTransactions();
    }
    
    bool hasBoughtItem() override {
        auto balance = CCStoreInventory::sharedStoreInventory()->
                       getItemBalance(PRODUCT_ID);
        auto res = balance == 1;
        return res;
    }
    
    /* FUNS */
    
    // TODO: rename "b"
    void callCallbackPurchase(bool b, string s) {
        if(!_callbackPurchase) {
            return;
        }
        _callbackPurchase(b, s);
        _callbackPurchase = nullptr;
    }
    
    void callCallbackRestore(bool b, string s) {
        if(!_callbackRestore) {
            return;
        }
        if(b) {
            bool bought = hasBoughtItem();
            if(bought) {
                _callbackRestore(true, "");
            } else {
                _callbackRestore(false, "NOTHING_TO_RESTORE");
            }
        } else {
            _callbackRestore(false, s);
        }
        _callbackRestore = nullptr;
    }
    
    /* HANDLERS */
    
    void onMarketPurchase(EventCustom *event) {
        callCallbackPurchase(true, "MarketPurchase");
    }
    
    void onMarketPurchaseCancelled(EventCustom *event) {
        callCallbackPurchase(false, "MarketPurchaseCancelled");
    }
    
    void onRestoreTransactionsFinished(EventCustom *event) {
        auto eventData = static_cast<__Dictionary*>(event->getUserData());
        auto successPtr = dynamic_cast<__Bool*>(
            eventData->objectForKey(CCStoreConsts::DICT_ELEMENT_SUCCESS)
        );
        bool success = successPtr->getValue();
        callCallbackRestore(success, "");
    }

    void onUnexpectedErrorInStore(EventCustom *event) {
        auto eventData = static_cast<__Dictionary*>(event->getUserData());
        auto errorMessage = dynamic_cast<__String*>(
            eventData->objectForKey(CCStoreConsts::DICT_ELEMENT_ERROR_MESSAGE)
        );
        if(errorMessage) {
            callCallbackPurchase(false, errorMessage->getCString());
        } else {
            callCallbackPurchase(false, "UnexpectedError");
        }
    }

};

EconomicsManager* EconomicsManager::getInstance() {
    static EconomicsManagerImpl instance;
    return &instance;
}






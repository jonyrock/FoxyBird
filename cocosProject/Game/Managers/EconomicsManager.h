#pragma once

#include "BasicManager.h"
#include "PlatformMacros.h"

#include <functional>
#include <string>


PLATFORM_DEPENDENT
class EconomicsManager : public BasicManager {
public:
    inline void setIsBought(bool value) { _isBought = value; }
    inline bool isBought() {
        if(_isBought) return true;
        if(hasBoughtItem()) {
            _isBought = true;
            return true;
        } else {
            return false;
        }
    }
    PLATFORM_DEPENDENT static EconomicsManager* getInstance();
    PLATFORM_DEPENDENT virtual bool init() override = 0;
    PLATFORM_DEPENDENT virtual bool hasBoughtItem() = 0;
    // TODO: special enum for types of errors (or search for such thing)
    PLATFORM_DEPENDENT virtual void askPurchase(std::function<void(bool, std::string)>) = 0;
    PLATFORM_DEPENDENT virtual void askRestore(std::function<void(bool, std::string)>) = 0;
private:
    bool _isBought = false;
};

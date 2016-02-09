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

#ifndef __CCVirtualGoodsStorage_H__
#define __CCVirtualGoodsStorage_H__

#include "CCVirtualItemStorage.h"
#include "CCVirtualGood.h"
#include "CCUpgradeVG.h"
#include "CCEquippableVG.h"

namespace soomla {
    /**
     This class is a definition of a Virtual Goods Storage.
     */
    class CCVirtualGoodsStorage : public CCVirtualItemStorage {
    public:
        static CCVirtualGoodsStorage* getInstance();
        
        CCVirtualGoodsStorage();
        
        virtual ~CCVirtualGoodsStorage();
        
        /**
         Removes any upgrade associated with the given `CCVirtualGood`.
         @param good `CCVirtualGood` to remove upgrade from.
         @param error Gets A `CCError` for error checking.
         */
        virtual void removeUpgrades(CCVirtualGood *good, CCError **error = NULL) { removeUpgrades(good, true, error); }
        /**
         Removes any upgrade associated with the given `CCVirtualGood`.
         @param good `CCVirtualGood` to remove upgrade from.
         @param notify true will also post event.
         @param error Gets A `CCError` for error checking.
         */
        virtual void removeUpgrades(CCVirtualGood *good, bool notify, CCError **error = NULL);
        
        /**
         Assigns a specific upgrade to the given virtual good.
         @param good `CCVirtualGood` to remove upgrade from.
         @param upgradeVG the upgrade to assign.
         @param error Gets A `CCError` for error checking.
         */
        virtual void assignCurrentUpgrade(CCVirtualGood *good, CCUpgradeVG *upgradeVG, CCError **error = NULL) { assignCurrentUpgrade(good, upgradeVG, true, error); }
        /**
         Assigns a specific upgrade to the given virtual good.
         @param good `CCVirtualGood` to remove upgrade from.
         @param upgradeVG the upgrade to assign.
         @param notify true will also post event.
         @param error Gets A `CCError` for error checking.
         */
        virtual void assignCurrentUpgrade(CCVirtualGood *good, CCUpgradeVG *upgradeVG, bool notify, CCError **error = NULL);
        
        /**
         Retrieves the current upgrade for the given virtual good.
         @param good the virtual good to retrieve upgrade for.
         @param error Gets A `CCError` for error checking.
         @return the current upgrade for the given virtual good, or NULL if one
         does not exist
         */
        virtual CCUpgradeVG *getCurrentUpgrade(CCVirtualGood *good, CCError **error = NULL);
        /**
         Checks the equipping status of the given `CCEquippableVG`.
         @param good The `CCEquippableVG` to check the status for.
         @param error Gets A `CCError` for error checking.
         @return true if the good is equipped, false otherwise
         */
        virtual bool isEquipped(CCEquippableVG *good, CCError **error = NULL);
        
        /**
         Equips the given `CCEquippableVG`.
         @param good The `CCEquippableVG` to equip.
         @param error Gets A `CCError` for error checking.
         */
        virtual void equip(CCEquippableVG *good, CCError **error = NULL) { equip(good, true, error); }
        /**
         Equips the given `CCEquippableVG`.
         @param good The `CCEquippableVG` to equip.
         @param notify true will also post event.
         @param error Gets A `CCError` for error checking.
         */
        virtual void equip(CCEquippableVG *good, bool notify, CCError **error = NULL);
        
        /**
         UnEquips the given `CCEquippableVG`.
         @param good The `CCEquippableVG` to unequip.
         @param error Gets A `CCError` for error checking.
         */
        virtual void unequip(CCEquippableVG *good, CCError **error = NULL) { unequip(good, true, error); }
        /**
         UnEquips the given `CCEquippableVG`.
         @param good The `CCEquippableVG` to unequip.
         @param notify true will also post event.
         @param error Gets A `CCError` for error checking.
         */
        virtual void unequip(CCEquippableVG *good, bool notify, CCError **error = NULL);
        
    protected:
        virtual const char* keyBalance(const char *itemId) const;
        virtual void postBalanceChangeEvent(CCVirtualItem *item, int balance, int amountAdded);
        
    private:
        void equipPriv(CCEquippableVG *good, bool equip, bool notify, CCError **error = NULL);
        
        static const char *keyGoodBalance(const char *itemId);
        static const char *keyGoodEquipped(const char *itemId);
        static const char *keyGoodUpgrade(const char *itemId);
    };
}

#endif // __CCVirtualGoodsStorage_H__

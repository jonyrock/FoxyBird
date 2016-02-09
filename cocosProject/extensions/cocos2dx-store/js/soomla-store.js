/**
 * Created by vedi on 15/04/15.
 */

(function () {

  /**
   * VirtualItem
   */
  var VirtualItem = Soomla.Models.VirtualItem = Soomla.declareClass('VirtualItem', {
    /**
     Gives your user the given amount of the specific virtual item.
     For example, when your users play your game for the first time you
     GIVE them 1000 gems.

     NOTE: This action is different than `PurchasableVirtualItem<`'s `buy()`:
     You use `give(int amount)`to give your user something for free.
     You use `buy()` to give your user something and get something in return.

     @param amount the amount of the specific item to be given.
     @param notify Notify of change in user's balance of current virtual item
     @return The balance after the giving process.
     */
    give: function (amount, notify) {
      Soomla.logError('IMPLEMENT ME!');
    },

    /**
     Takes from your user the given amount of the specific virtual item.
     For example, when you want to downgrade a virtual good, you take the upgrade.

     @param amount The amount of the specific item to be taken.
     @param notify Notify of change in user's balance of current virtual item
     @return The balance after the taking process.
     */
    take: function take(amount, notify) {
      Soomla.logError('IMPLEMENT ME!');
    },

    /**
     Resets this Virtual Item's balance to the given balance.

     @param balance The balance of the current virtual item.
     @param notify Notify of change in user's balance of current virtual item.
     @return The balance after the reset process.
     */
    resetBalance: function resetBalance(balance, notify) {
      Soomla.logError('IMPLEMENT ME!');
    },

    /**
     Will fetch the balance for the current VirtualItem according to its type.

     @return The balance.
     */
    getBalance: function getBalance() {
      Soomla.logError('IMPLEMENT ME!');
    },

    /**
     Save this instance with changes that were made to it.
     The saving is done in the metadata in StoreInfo and being persisted to the local DB.
     */
    save: function () {
      Soomla.storeInfo.saveItem(this);
    }
  }, Soomla.Models.SoomlaEntity);

  /**
   * VirtualCategory
   */
  var VirtualCategory = Soomla.Models.VirtualCategory = Soomla.declareClass('VirtualCategory', {
    name: '',
    goods_itemIds: null
  }, Soomla.Models.Domain);

  /**
   * MarketItem
   */
  var MarketItem = Soomla.Models.MarketItem = Soomla.declareClass('MarketItem', {
    productId: null,
    //androidId: null,
    //iosId: null,
    consumable: null,
    price: null,
    marketPrice: 0,
    marketTitle: null,
    marketDesc: null,
    marketCurrencyCode: null,
    marketPriceMicros: 0,

    onCreate: function () {
      if (!this.productId) {
        if (Soomla.platform.isAndroid()) {
          this.productId = this.androidId;
        } else if (Soomla.platform.isIos()) {
          this.productId = this.iosId;
        }
      }
    }
  }, Soomla.Models.Domain);
  MarketItem.Consumable = {
    NONCONSUMABLE: 0,
    CONSUMABLE: 1,
    SUBSCRIPTION: 2
  };

  var PURCHASE_TYPE = {
    MARKET: 'market',
    VI: 'virtualItem'
  };

  /**
   * StoreConsts
   */
  var StoreConsts = Soomla.Models.StoreConsts = {
    EVENT_BILLING_NOT_SUPPORTED: 'CCStoreEventHandler::onBillingNotSupported',
    EVENT_BILLING_SUPPORTED: 'CCStoreEventHandler::onBillingSupported',
    EVENT_CURRENCY_BALANCE_CHANGED: 'CCStoreEventHandler::onCurrencyBalanceChanged',
    EVENT_GOOD_BALANCE_CHANGED: 'CCStoreEventHandler::onGoodBalanceChanged',
    EVENT_GOOD_EQUIPPED: 'CCStoreEventHandler::onGoodEquipped',
    EVENT_GOOD_UNEQUIPPED: 'CCStoreEventHandler::onGoodUnEquipped',
    EVENT_GOOD_UPGRADE: 'CCStoreEventHandler::onGoodUpgrade',
    EVENT_ITEM_PURCHASED: 'CCStoreEventHandler::onItemPurchased',
    EVENT_ITEM_PURCHASE_STARTED: 'CCStoreEventHandler::onItemPurchaseStarted',
    EVENT_MARKET_PURCHASE_CANCELED: 'CCStoreEventHandler::onMarketPurchaseCancelled',
    EVENT_MARKET_PURCHASE: 'CCStoreEventHandler::onMarketPurchase',
    EVENT_MARKET_PURCHASE_STARTED: 'CCStoreEventHandler::onMarketPurchaseStarted',
    EVENT_MARKET_ITEMS_REFRESHED: 'CCStoreEventHandler::onMarketItemsRefreshed',
    EVENT_MARKET_ITEMS_REFRESH_STARTED: 'CCStoreEventHandler::onMarketItemsRefreshStarted',
    EVENT_MARKET_ITEMS_REFRESH_FAILED: 'CCStoreEventHandler::onMarketItemsRefreshFailed',
    EVENT_MARKET_PURCHASE_VERIFICATION: 'CCStoreEventHandler::onMarketPurchaseVerification',
    EVENT_RESTORE_TRANSACTION_FINISHED: 'CCStoreEventHandler::onRestoreTransactionsFinished',
    EVENT_RESTORE_TRANSACTION_STARTED: 'CCStoreEventHandler::onRestoreTransactionsStarted',
    EVENT_UNEXPECTED_ERROR_IN_STORE: 'CCStoreEventHandler::onUnexpectedErrorInStore',
    EVENT_SOOMLA_STORE_INITIALIZED: 'CCStoreEventHandler::onSoomlaStoreInitialized',
    EVENT_MARKET_REFUND: 'CCStoreEventHandler::onMarketRefund',
    EVENT_IAB_SERVICE_STARTED: 'CCStoreEventHandler::onIabServiceStarted',
    EVENT_IAB_SERVICE_STOPPED: 'CCStoreEventHandler::onIabServiceStopped'
  };

  /**
   * PurchasableVirtualItem
   */
  var PurchasableVirtualItem = Soomla.Models.PurchasableVirtualItem = Soomla.declareClass('PurchasableVirtualItem', {
    purchasableItem: null,

    onCreate: function () {
      this.purchasableItem.associatedItemId = this.itemId;
    },

    /**
     Checks if there is enough funds to afford the `CCPurchasableVirtualItem`.
     This action uses the associated `CCPurchaseType` to perform the check.
     @return True if there are enough funds to afford the virtual item
     with the given item id
     */
    canAfford: function canAfford() {
      this.purchasableItem.canAfford();
    },

    /**
     Buys the `CCPurchasableVirtualItem`, after checking if the user is in a state that
     allows him/her to buy. This action uses the associated `CCPurchaseType` to perform
     the purchase.

     @param payload a string you want to be assigned to the purchase.
     This string is saved in a static variable and will be given bacl to you
     when the purchase is completed..
     */
    buy: function buy(payload) {
      if (!this.canBuy()) {
        return;
      }

      if (!payload) {
        payload = '';
      }

      this.purchasableItem.buy(payload);
    },

    /**
     Determines if user is in a state that allows him/her to buy a specific
     `CCVirtualItem`.
     */
    canBuy: function canBuy() {
      return false;
    }
  }, VirtualItem);

  /**
   * VirtualCurrency
   */
  var VirtualCurrency = Soomla.Models.VirtualCurrency = Soomla.declareClass('VirtualCurrency', {
    /**
     See parent
     */
    give: function give(amount, notify) {
      return Soomla.virtualCurrencyStorage.add(this, amount, notify);
    },
    /**
     See parent
     */
    take: function take(amount, notify) {
      return Soomla.virtualCurrencyStorage.remove(this, amount, notify);
    },

    /**
     See parent
     */
    resetBalance: function (balance, notify) {
      return Soomla.virtualCurrencyStorage.setBalance(this, balance, notify);
    },

    /**
     See parent
     */
    getBalance: function getBalance() {
      return Soomla.virtualCurrencyStorage.getBalance(this);
    }
  }, VirtualItem);

  /**
   * VirtualCurrencyPack
   */
  var VirtualCurrencyPack = Soomla.Models.VirtualCurrencyPack = Soomla.declareClass('VirtualCurrencyPack', {
    currency_amount: 0,
    currency_itemId: null,

    /**
     See parent
     */
    canBuy: function canBuy() {
      return true;
    },

    /**
     See parent
     */
    give: function (amount, notify) {
      var currencyId = this.currency_itemId;
      var currency = Soomla.storeInfo.getItemByItemId(currencyId);

      if (!currency) {
        Soomla.logError('VirtualCurrency with itemId: ' + currencyId + ' doesn\'t exist! Can\'t give this pack.');
        return 0;
      }

      return Soomla.virtualCurrencyStorage.add(currency, this.currency_amount * amount, notify);
    },

    /**
     See parent
     */
    take: function take(amount, notify) {
      var currencyId = this.currency_itemId;
      var currency = Soomla.storeInfo.getItemByItemId(currencyId);

      if (!currency) {
        Soomla.logError('VirtualCurrency with itemId: ' + currencyId + ' doesn\'t exist! Can\'t take this pack.');
        return 0;
      }

      return Soomla.virtualCurrencyStorage.remove(currency, this.currency_amount * amount, notify);
    },

    /**
     See parent
     */
    resetBalance: function resetBalance(balance, notify) {
      // Not supported for VirtualCurrencyPacks !
      Soomla.logError('Someone tried to reset balance of CurrencyPack. That\'s not right.');
      return 0;
    },

    /**
     See parent
     */
    getBalance: function getBalance() {
      // Not supported for VirtualCurrencyPacks !
      Soomla.logError('Someone tried to check balance of CurrencyPack. That\'s not right.');
      return 0;
    }

  }, PurchasableVirtualItem);

  /**
   * VirtualGood
   */
  var VirtualGood = Soomla.Models.VirtualGood = Soomla.declareClass('VirtualGood', {
    /**
     See parent
     */
    resetBalance: function resetBalance(balance, notify) {
      return Soomla.virtualGoodsStorage.setBalance(this, balance, notify);
    },
    /**
     See parent
     */
    getBalance: function getBalance() {
      return Soomla.virtualGoodsStorage.getBalance(this);

    }
  }, PurchasableVirtualItem);

  /**
   * LifetimeVG
   */
  var LifetimeVG = Soomla.Models.LifetimeVG = Soomla.declareClass('LifetimeVG', {
    /**
     See parent
     */
    canBuy: function () {
      var balance = Soomla.virtualGoodsStorage.getBalance(this);

      return balance < 1;
    },

    /**
     See parent
     */
    give: function (amount, notify) {
      if (amount > 1) {
        Soomla.logDebug('You tried to give more than one LifetimeVG. \
                                    Will try to give one anyway.');
        amount = 1;
      }

      var balance = Soomla.virtualGoodsStorage.getBalance(this);

      if (balance < 1) {
        return Soomla.virtualGoodsStorage.add(this, amount, notify);
      }
      return 1;
    },

    /**
     See parent
     */
    take: function (amount, notify) {
      if (amount > 1) {
        amount = 1;
      }

      var balance = Soomla.virtualGoodsStorage.getBalance(this);

      if (balance > 0) {
        return Soomla.virtualGoodsStorage.remove(this, amount, notify);
      }
      return 0;
    }
  }, VirtualGood);

  /**
   * EquippableVG
   */
  var EquippableVG = Soomla.Models.EquippableVG = Soomla.declareClass('EquippableVG', {
    equipping: null,

    /**
     Equips the current `CCEquippableVG`.
     The equipping is done according to the equipping model ('GLOBAL', 'CATEGORY', or 'LOCAL').
     @param notify if true, the relevant event will be posted when equipped.
     be returned here
     */
    equip: function (notify) {
      notify = notify || _.isUndefined(notify);

      // only if the user has bought this EquippableVG, the EquippableVG is equipped.
      if (Soomla.virtualGoodsStorage.getBalance(this) > 0) {
        var equippingModel = this.equipping;
        if (equippingModel === EquippableVG.EquippingModel.CATEGORY) {
          var itemId = this.itemId;
          var category = Soomla.storeInfo.getCategoryForVirtualGood(itemId);
          if (!category) {
            Soomla.logError('Tried to unequip all other category VirtualGoods but there was no associated category. \
                  virtual good itemId: ' + itemId);
            return;
          }

          var goodItemIds = category.goods_itemIds;
          _.forEach(goodItemIds, function (goodItemId) {
            var equippableVG = Soomla.storeInfo.getItemByItemId(goodItemId);
            if (!equippableVG) {
              Soomla.logError('On equip, couldn\'t find one of the itemIds in the category. ' +
              'Continuing to the next one. itemId: ' + goodItemId);
            }
            else if (equippableVG !== this) {
              equippableVG.unequip(notify);
            }
          }, this);

        } else if (equippingModel == EquippableVG.EquippingModel.GLOBAL) {
          var virtualGoods = Soomla.storeInfo.getGoods();
          _.forEach(virtualGoods, function (virtualGood) {
            if ((virtualGood != this) && (virtualGood.className === 'EquippableVG')) {
              virtualGood.unequip(notify);
            }
          }, this);
        }

        Soomla.virtualGoodsStorage.equip(this, notify);
      }
      else {
        Soomla.logError('You tried to equip virtual good with itemId: ' + this.itemId + ' but you don\'t have any of it.');
      }
    },

    /**
     Unequips the current `CCEquippableVG`
     @param notify if true, the relevant event will be posted when equipped.
     be returned here
     */
    unequip: function (notify) {
      notify = notify || _.isUndefined(notify);
      Soomla.virtualGoodsStorage.unequip(this, notify);
    }

  }, LifetimeVG);
  EquippableVG.EquippingModel = {
    LOCAL: 'local',
    CATEGORY: 'category',
    GLOBAL: 'global'
  };

  /**
   * SingleUseVG
   */
  var SingleUseVG = Soomla.Models.SingleUseVG = Soomla.declareClass('SingleUseVG', {

    /**
     See parent
     */
    canBuy: function () {
      return true;
    },

    /**
     See parent
     */
    give: function (amount, notify) {
      return Soomla.virtualGoodsStorage.add(this, amount, notify);

    },

    /**
     See parent
     */
    take: function (amount, notify) {
      return Soomla.virtualGoodsStorage.remove(this, amount, notify);
    }

  }, VirtualGood);

  /**
   * SingleUsePackVG
   */
  var SingleUsePackVG = Soomla.Models.SingleUsePackVG = Soomla.declareClass('SingleUsePackVG', {
    good_itemId: null,
    good_amount: null,

    /**
     See parent
     */
    canBuy: function () {
      return true;
    },

    /**
     See parent
     */
    give: function (amount, notify) {
      var goodItemId = this.good_itemId;
      var good = Soomla.storeInfo.getItemByItemId(goodItemId);

      if (!good) {
        Soomla.logError('SingleUseVG with itemId: ' + goodItemId + ' doesn\'t exist! Can\'t give this pack.');
        return 0;
      }

      return Soomla.virtualGoodsStorage.add(good, this.good_amount * amount, notify);
    },

    /**
     See parent
     */
    take: function (amount, notify) {
      var goodItemId = this.good_itemId;
      var good = Soomla.storeInfo.getItemByItemId(goodItemId);

      if (!good) {
        Soomla.logError('SingleUseVG with itemId: ' + goodItemId + ' doesn\'t exist! Can\'t give this pack.');
        return 0;
      }

      return Soomla.virtualGoodsStorage.remove(good, this.good_amount * amount, notify);
    },

    /**
     See parent
     */
    resetBalance: function (balance, notify) {
      // Not supported for SingleUsePackVGs !
      Soomla.logError('Someone tried to reset balance of GoodPack. That\'s not right.');
      return 0;
    },

    /**
     See parent
     */
    getBalance: function () {
      // Not supported for SingleUsePackVGs !
      Soomla.logError('Someone tried to check balance of GoodPack. That\'s not right.');
      return 0;
    }
  }, VirtualGood);

  /**
   * UpgradeVG
   */
  var UpgradeVG = Soomla.Models.UpgradeVG = Soomla.declareClass('UpgradeVG', {
    good_itemId: null,
    prev_itemId: null,
    next_itemId: null,

    /**
     See parent
     */
    canBuy: function () {
      var goodItemId = this.good_itemId;
      var good = Soomla.storeInfo.getItemByItemId(goodItemId);
      if (!good) {
        Soomla.logError('VirtualGood with itemId: ' + goodItemId + ' doesn\'t exist! Returning false (can\'t buy).');
        return false;
      }

      var upgradeVG = Soomla.virtualGoodsStorage.getCurrentUpgrade(good);

      return ((!upgradeVG && (_.isNull(this.prev_itemId) || _.isEmpty(this.prev_itemId))) ||
        (upgradeVG && (upgradeVG.next_itemId === this.itemId) || upgradeVG.prev_itemId === this.itemId))
        && this.__super__.canBuy.apply(this, arguments);
    },

    /**
     See parent
     */
    give: function (amount, notify) {
      var goodItemId = this.good_itemId;
      Soomla.logDebug('Assigning ' + this.name + ' to: ' + this.itemId);

      var good = Soomla.storeInfo.getItemByItemId(goodItemId);
      if (!good) {
        Soomla.logError('VirtualGood with itemId: ' + goodItemId + ' doesn\'t exist! Can\'t upgrade.');
        return 0;
      }

      Soomla.virtualGoodsStorage.assignCurrentUpgrade(good, this, notify);

      return this.__super__.give.apply(this, arguments);
    },

    /**
     See parent
     */
    take: function (amount, notify) {
      var goodItemId = this.good_itemId;

      var good = Soomla.storeInfo.getItemByItemId(goodItemId);
      if (!good) {
        Soomla.logError('VirtualGood with itemId: ' + goodItemId + ' doesn\'t exist! Can\'t downgrade.');
        return 0;
      }

      var upgradeVG = Soomla.virtualGoodsStorage.getCurrentUpgrade(good, error);

      // Case: Upgrade is not assigned to this Virtual Good
      if (upgradeVG !== this) {
        Soomla.logError('You can\'t take an upgrade that\'s not currently assigned. The UpgradeVG ' + this.name +
        ' is not assigned to the VirtualGood: ' + good.name);
        return 0;
      }

      if (!(_.isNull(this.prev_itemId) || _.isEmpty(this.prev_itemId))) {
        var prevItemId = this.prev_itemId;
        var prevUpgradeVG = Soomla.storeInfo.getItemByItemId(prevItemId);

        // Case: downgrade is not possible because previous upgrade does not exist
        if (!prevUpgradeVG) {
          Soomla.logError('Previous UpgradeVG with itemId: ' + prevItemId + ' doesn\'t exist! Can\'t downgrade.');
          return 0;
        }

        // Case: downgrade is successful!
        Soomla.logDebug('Downgrading ' + good.name + ' to: ' + prevUpgradeVG.name);

        Soomla.virtualGoodsStorage.assignCurrentUpgrade(good, prevUpgradeVG, notify);
      }

      // Case: first Upgrade in the series - so we downgrade to NO upgrade.
      else {
        Soomla.logError('Downgrading ' + good.name + ' to NO-UPGRADE');
        Soomla.virtualGoodsStorage.removeUpgrades(good, notify, error);
      }

      return this.__super__.take.apply(this, arguments);
    }
  }, LifetimeVG);

  /**
   * PurchaseType
   */
  var PurchaseType = Soomla.Models.PurchaseType = Soomla.declareClass('PurchaseType', {
    purchaseType: null,
    associatedItemId: null,

    /**
     Buys the purchasable virtual item.
     Implementation in subclasses will be according to specific type of
     purchase.

     @param payload string you want to be assigned to the purchase. This
     string is saved in a static variable and will be given bacl to you when
     the purchase is completed.
     */
    buy: function (payload) {
      Soomla.logError('IMPLEMENT ME!');
    },

    /**
     Checks if there is enough funds to afford the `CCPurchasableVirtualItem`.
     Implementation in subclasses will be according to specific type of purchase.
     @return True if there are enough funds to afford the virtual item
     with the given item id
     */
    canAfford: function () {
      Soomla.logError('IMPLEMENT ME!');
    }
  });

  /**
   * PurchaseWithMarket
   */
  var PurchaseWithMarket = Soomla.Models.PurchaseWithMarket = Soomla.declareClass('PurchaseWithMarket', {
    purchaseType: PURCHASE_TYPE.MARKET,
    marketItem: null,

    /**
     See parent
     */
    buy: function (payload) {
      Soomla.logDebug('Starting in-app purchase for productId: ' + this.marketItem.productId);

      if (_.isNull(payload)) {
        payload = '';
      }

      var pvi = Soomla.storeInfo.getItemByItemId(this.associatedItemId);
      Soomla.fireSoomlaEvent(StoreConsts.EVENT_ITEM_PURCHASE_STARTED, [pvi, true]);
      Soomla.soomlaStore.buyMarketItem(this.marketItem.productId, payload);
    },

    /**
     See parent
     */
    canAfford: function () {
      return true;
    }

  }, PurchaseType);

  PurchaseWithMarket.createWithMarketItem = function (productId, price) {
    var marketItem = MarketItem.create({
      productId: productId,
      consumable: MarketItem.Consumable.CONSUMABLE,
      price: price
    });
    return PurchaseWithMarket.create({marketItem: marketItem});
  };

  /**
   * PurchaseWithVirtualItem
   */
  var PurchaseWithVirtualItem = Soomla.Models.PurchaseWithVirtualItem = Soomla.declareClass('PurchaseWithVirtualItem', {
    purchaseType: PURCHASE_TYPE.VI,
    pvi_itemId: null,
    pvi_amount: null,

    /**
     See parent
     */
    buy: function (payload) {
      var associatedItemId = this.associatedItemId;
      if (!payload) {
        payload = '';
      }
      var associatedItem = Soomla.storeInfo.getItemByItemId(associatedItemId);
      if (!associatedItem) {
        Soomla.logError('Trying to buy an non-existing associated item: ' + associatedItemId);
        return;
      }

      var targetItemId = this.pvi_itemId;
      var amount = this.pvi_amount;
      Soomla.logDebug('Trying to buy a ' + associatedItem.name + ' with ' + amount + ' pieces of ' + targetItemId);

      var item = Soomla.storeInfo.getItemByItemId(targetItemId);
      if (!item) {
        Soomla.logError('Target virtual item ' + targetItemId + ' doesn\'t exist !');
        return;
      }

      Soomla.fireSoomlaEvent(StoreConsts.EVENT_ITEM_PURCHASE_STARTED, [associatedItem, true]);

      var balance = item.getBalance();

      if (balance < amount) {
        Soomla.logError('You tried to buy with itemId: ' + item.itemId + ' but you don\'t have enough funds to buy it.');
        return;
      }

      item.take(amount);

      associatedItem.give(1);

      Soomla.fireSoomlaEvent(StoreConsts.EVENT_ITEM_PURCHASED, [associatedItem, payload, true]);
    },

    /**
     See parent
     */
    canAfford: function () {
      var associatedItemId = this.associatedItemId || this.pvi_itemId;

      var associatedItem = Soomla.storeInfo.getItemByItemId(associatedItemId);
      if (!associatedItem) {
        Soomla.logError('Trying to buy an non-existing associated item: ' + associatedItemId);
        return;
      }

      var targetItemId = this.pvi_itemId;
      var amount = this.pvi_amount;
      Soomla.logDebug('Checking affordability of ' + associatedItem.name + ' with ' + amount + ' pieces of ' + targetItemId);

      var item = Soomla.storeInfo.getItemByItemId(targetItemId);
      if (!item) {
        Soomla.logError('Target virtual item ' + targetItemId + ' doesn\'t exist !');
        return false;
      }

      var balance = item.getBalance();

      return balance >= amount;
    }

  }, PurchaseType);

  /**
   * VirtualItemReward
   */
  var VirtualItemReward = Soomla.Models.VirtualItemReward = Soomla.declareClass('VirtualItemReward', {
    amount: null,
    associatedItemId: null,
    takeInner: function takeInner() {
      Soomla.storeInventory.takeItem(this.associatedItemId, this.amount);
      return true;
    },
    giveInner: function giveInner() {
      Soomla.storeInventory.giveItem(this.associatedItemId, this.amount);
    }
  }, Soomla.Models.Reward);


  /**
   * VirtualItemStorage
   */
  var VirtualItemStorage = Soomla.VirtualItemStorage = Soomla.declareClass('VirtualItemStorage', {
    getBalance: function getBalance(item) {
      var itemId = item.itemId;
      var key = this.keyBalance(itemId);
      var val = Soomla.keyValueStorage.getValue(key);

      var balance = (!_.isUndefined(val) && !_.isEmpty(val)) ? parseInt(val) : 0;

      Soomla.logDebug('the balance for ' + itemId + ' is ' + balance);

      return balance;
    },

    setBalance: function setBalance(item, balance, notify) {
      notify = notify || _.isUndefined(notify);
      var oldBalance = this.getBalance(item);
      if (oldBalance === balance) {
        return balance;
      }

      var itemId = item.itemId;
      var key = this.keyBalance(itemId);

      Soomla.keyValueStorage.setValue(key, balance);

      if (notify) {
        this.postBalanceChangeEvent(item, balance, 0);
      }

      return balance;
    },

    add: function add(item, amount, notify) {
      notify = notify || _.isUndefined(notify);
      var itemId = item.itemId;
      var balance = this.getBalance(item);
      if (balance < 0) { /* in case the user "adds" a negative value */
        balance = 0;
        amount = 0;
      }

      var newBalance = balance + amount;
      var key = this.keyBalance(itemId);

      Soomla.keyValueStorage.setValue(key, newBalance);

      if (notify) {
        this.postBalanceChangeEvent(item, newBalance, amount);
      }

      return newBalance;
    },

    remove: function remove(item, amount, notify) {
      notify = notify || _.isUndefined(notify);
      var itemId = item.itemId;
      var balance = this.getBalance(item) - amount;
      if (balance < 0) {
        balance = 0;
        amount = 0;
      }

      var key = this.keyBalance(itemId);

      Soomla.keyValueStorage.setValue(key, balance);

      if (notify) {
        this.postBalanceChangeEvent(item, balance, -1 * amount);
      }

      return balance;
    },

    keyBalance: function keyBalance(itemId) {
      Soomla.logError('OVERRIDE ME!')
    },

    postBalanceChangeEvent: function (item, balance, amountAdded) {
      Soomla.logError('OVERRIDE ME!')
    }
  });

  /**
   * VirtualCurrencyStorage
   */
  var VirtualCurrencyStorage = Soomla.VirtualCurrencyStorage = Soomla.declareClass('VirtualCurrencyStorage', {
    keyBalance: function keyBalance(itemId) {
      return this.keyCurrencyBalance(itemId);
    },
    postBalanceChangeEvent: function postBalanceChangeEvent(item, balance, amountAdded) {
      Soomla.fireSoomlaEvent(StoreConsts.EVENT_CURRENCY_BALANCE_CHANGED, [item, balance, amountAdded]);
    },
    keyCurrencyBalance: function keyCurrencyBalance(itemId) {
      return 'currency.' + itemId + '.balance';
    }
  }, VirtualItemStorage);

  /**
   * NativeVirtualCurrencyStorage
   */
  var NativeVirtualCurrencyStorage = Soomla.NativeVirtualCurrencyStorage = Soomla.declareClass('NativeVirtualCurrencyStorage', {

    getBalance: function getBalance(item) {
      var itemId = item.itemId;

      Soomla.logDebug('SOOMLA/COCOS2DX Calling getBalance with: ' + itemId);
      var retParams = Soomla.callNative({
        method: 'CCNativeVirtualCurrencyStorage::getBalance',
        itemId: itemId
      });

      return retParams['return'] || 0;
    },

    setBalance: function setBalance(item, balance, notify) {
      notify = notify || _.isUndefined(notify);
      var itemId = item.itemId;

      Soomla.logDebug('SOOMLA/COCOS2DX Calling setBalance with: ' + itemId);

      var retParams = Soomla.callNative({
        method: 'CCNativeVirtualCurrencyStorage::setBalance',
        itemId: itemId,
        balance: balance,
        notify: notify
      });

      return retParams['return'] || 0;
    },

    add: function add(item, amount, notify) {
      notify = notify || _.isUndefined(notify);
      var itemId = item.itemId;

      Soomla.logDebug('SOOMLA/COCOS2DX Calling add with: ' + itemId);

      var retParams = Soomla.callNative({
        method: 'CCNativeVirtualCurrencyStorage::add',
        itemId: itemId,
        amount: amount,
        notify: notify
      });

      return retParams['return'] || 0;
    },

    remove: function remove(item, amount, notify) {
      notify = notify || _.isUndefined(notify);
      var itemId = item.itemId;

      Soomla.logDebug('SOOMLA/COCOS2DX Calling remove with: ' + itemId);

      var retParams = Soomla.callNative({
        method: 'CCNativeVirtualCurrencyStorage::remove',
        itemId: itemId,
        amount: amount,
        notify: notify
      });

      return retParams['return'] || 0;
    }

  }, VirtualCurrencyStorage);

  Soomla.virtualCurrencyStorage = Soomla.platform.isNativeSupported() ? NativeVirtualCurrencyStorage.create() : VirtualCurrencyStorage.create();

  /**
   * VirtualGoodsStorage
   */
  var VirtualGoodsStorage = Soomla.VirtualGoodsStorage = Soomla.declareClass('VirtualGoodsStorage', {
    /**
     Removes any upgrade associated with the given `CCVirtualGood`.
     @param good `CCVirtualGood` to remove upgrade from.
     @param notify true will also post event.
     */
    removeUpgrades: function removeUpgrades(good, notify) {
      notify = notify || _.isUndefined(notify);
      var itemId = good.itemId;
      var key = this.keyGoodUpgrade(itemId);

      Soomla.keyValueStorage.deleteKeyValue(key);

      if (notify) {
        Soomla.fireSoomlaEvent(StoreConsts.EVENT_GOOD_UPGRADE, [good]);
      }
    },

    /**
     Assigns a specific upgrade to the given virtual good.
     @param good `CCVirtualGood` to remove upgrade from.
     @param upgradeVG the upgrade to assign.
     @param notify true will also post event.
     */
    assignCurrentUpgrade: function assignCurrentUpgrade(good, upgradeVG, notify) {
      notify = notify || _.isUndefined(notify);
      var upgrade = this.getCurrentUpgrade(good);
      if (upgrade && upgrade.itemId === upgradeVG.itemId) {
        return;
      }

      var itemId = good.itemId;
      var key = this.keyGoodUpgrade(itemId);
      var upItemId = upgradeVG.itemId;

      Soomla.keyValueStorage.setValue(key, upItemId);

      if (notify) {
        Soomla.fireSoomlaEvent(StoreConsts.EVENT_GOOD_UPGRADE, [good, upgradeVG]);
      }
    },

    /**
     Retrieves the current upgrade for the given virtual good.
     @param good the virtual good to retrieve upgrade for.
     @return the current upgrade for the given virtual good, or NULL if one
     does not exist
     */
    getCurrentUpgrade: function getCurrentUpgrade(good) {
      var itemId = good.itemId;
      var key = this.keyGoodUpgrade(itemId);

      var upItemId = Soomla.keyValueStorage.getValue(key);

      if (!upItemId) {
        Soomla.logDebug('You tried to fetch the current upgrade of ' + itemId + ' but there\'s not upgrade to it.');
        return null;
      }

      var item = Soomla.storeInfo.getItemByItemId(upItemId);

      return item || null;
    },

    /**
     Checks the equipping status of the given `CCEquippableVG`.
     @param good The `CCEquippableVG` to check the status for.
     @return boolean true if the good is equipped, false otherwise
     */
    isEquipped: function isEquipped(good) {
      var itemId = good.itemId;
      var key = this.keyGoodEquipped(itemId);
      var val = Soomla.keyValueStorage.getValue(key);

      return !!val;
    },

    /**
     Equips the given `CCEquippableVG`.
     @param good The `CCEquippableVG` to equip.
     @param notify true will also post event.
     */
    equip: function equip(good, notify) {
      notify = notify || _.isUndefined(notify);
      if (this.isEquipped(good)) {
        return;
      }

      this.equipPriv(good, true, notify);
    },

    /**
     UnEquips the given `CCEquippableVG`.
     @param good The `CCEquippableVG` to unequip.
     @param notify true will also post event.
     */
    unequip: function unequip(good, notify) {
      notify = notify || _.isUndefined(notify);
      if (!this.isEquipped(good)) {
        return;
      }

      this.equipPriv(good, false, notify);
    },


    keyBalance: function keyBalance(itemId) {
      return this.keyGoodBalance(itemId);
    },

    postBalanceChangeEvent: function postBalanceChangeEvent(item, balance, amountAdded) {
      Soomla.fireSoomlaEvent(StoreConsts.EVENT_GOOD_BALANCE_CHANGED, [item, balance, amountAdded]);
    },

    equipPriv: function equipPriv(good, equip, notify) {
      var itemId = good.itemId;
      var key = this.keyGoodEquipped(itemId);

      if (equip) {
        Soomla.keyValueStorage.setValue(key, 'yes');
        if (notify) {
          Soomla.fireSoomlaEvent(StoreConsts.EVENT_GOOD_EQUIPPED, [good]);
        }
      } else {
        Soomla.keyValueStorage.deleteKeyValue(key);
        if (notify) {
          Soomla.fireSoomlaEvent(StoreConsts.EVENT_GOOD_UNEQUIPPED, [good]);
        }
      }
    },

    keyGoodBalance: function keyGoodBalance(itemId) {
      return 'good.' + itemId + '.balance';
    },

    keyGoodEquipped: function keyGoodEquipped(itemId) {
      return 'good.' + itemId + '.equipped';
    },

    keyGoodUpgrade: function keyGoodUpgrade(itemId) {
      return 'good.' + itemId + '.currentUpgrade';
    }

  }, VirtualItemStorage);

  /**
   Implements the `CCVirtualGoodsStorage` using the bridge to talk
   with the native implementation of VirtualGoodsStorage

   See parent for all functions.
   */
  var NativeVirtualGoodsStorage = Soomla.NativeVirtualGoodsStorage = Soomla.declareClass('NativeVirtualGoodsStorage', {

    getBalance: function getBalance(item) {
      var itemId = item.itemId;

      Soomla.logDebug('SOOMLA/COCOS2DX Calling getBalance with: ' + itemId);

      var retParams = Soomla.callNative({
        method: 'CCNativeVirtualGoodsStorage::getBalance',
        itemId: itemId
      });

      return retParams['return'] || 0;
    },

    setBalance: function setBalance(item, balance, notify) {
      notify = notify || _.isUndefined(notify);
      var itemId = item.itemId;

      Soomla.logDebug('SOOMLA/COCOS2DX Calling setBalance with: ' + itemId);

      var retParams = Soomla.callNative({
        method: 'CCNativeVirtualGoodsStorage::setBalance',
        itemId: itemId,
        balance: balance,
        notify: notify
      });

      return retParams['return'] || 0;
    },

    add: function add(item, amount, notify) {
      notify = notify || _.isUndefined(notify);
      var itemId = item.itemId;

      Soomla.logDebug('SOOMLA/COCOS2DX Calling add with: ' + itemId);

      var retParams = Soomla.callNative({
        method: 'CCNativeVirtualGoodsStorage::add',
        itemId: itemId,
        amount: amount,
        notify: notify
      });

      return retParams['return'] || 0;
    },

    remove: function remove(item, amount, notify) {
      notify = notify || _.isUndefined(notify);
      var itemId = item.itemId;

      Soomla.logDebug('SOOMLA/COCOS2DX Calling remove with: ' + itemId);

      var retParams = Soomla.callNative({
        method: 'CCNativeVirtualGoodsStorage::remove',
        itemId: itemId,
        amount: amount,
        notify: notify
      });

      return retParams['return'] || 0;
    },

    removeUpgrades: function removeUpgrades(good, notify) {
      notify = notify || _.isUndefined(notify);
      var itemId = good.itemId;

      Soomla.logDebug('SOOMLA/COCOS2DX Calling removeUpgrades with: ' + itemId);

      Soomla.callNative({
        method: 'CCNativeVirtualGoodsStorage::removeUpgrades',
        itemId: itemId,
        notify: notify
      });
    },

    assignCurrentUpgrade: function assignCurrentUpgrade(good, upgradeVG, notify) {
      notify = notify || _.isUndefined(notify);
      var itemId = good.itemId;
      var upgradeItemId = upgradeVG.itemId;


      Soomla.logDebug('SOOMLA/COCOS2DX Calling assignCurrentUpgrade with: ' + itemId);

      Soomla.callNative({
        method: 'CCNativeVirtualGoodsStorage::assignCurrentUpgrade',
        itemId: itemId,
        upgradeItemId: upgradeItemId,
        notify: notify
      });
    },

    getCurrentUpgrade: function getCurrentUpgrade(good) {
      var itemId = good.itemId;

      Soomla.logDebug('SOOMLA/COCOS2DX Calling getCurrentUpgrade with: ' + itemId);

      var retParams = Soomla.callNative({
        method: 'CCNativeVirtualGoodsStorage::getCurrentUpgrade',
        itemId: itemId
      });

      var retItemId = retParams['return'];

      return retItemId ? Soomla.storeInfo.getItemByItemId(retItemId) : null;
    },

    isEquipped: function isEquipped(good) {
      var itemId = good.itemId;

      Soomla.logDebug('SOOMLA/COCOS2DX Calling isEquipped with: ' + itemId);

      var retParams = Soomla.callNative({
        method: 'CCNativeVirtualGoodsStorage::isEquipped',
        itemId: itemId
      });

      return retParams['return'] || false;
    },

    equip: function equip(good, notify) {
      notify = notify || _.isUndefined(notify);
      var itemId = good.itemId;

      Soomla.logDebug('SOOMLA/COCOS2DX Calling equip with: ' + itemId);

      Soomla.callNative({
        method: 'CCNativeVirtualGoodsStorage::equip',
        itemId: itemId,
        notify: notify
      });
    },

    unequip: function unequip(good, notify) {
      notify = notify || _.isUndefined(notify);
      var itemId = good.itemId;

      Soomla.logDebug('SOOMLA/COCOS2DX Calling unequip with: ' + itemId);

      Soomla.callNative({
        method: 'CCNativeVirtualGoodsStorage::unequip',
        itemId: itemId,
        notify: notify
      });
    }
  }, VirtualGoodsStorage);

  Soomla.virtualGoodsStorage = Soomla.platform.isNativeSupported() ? NativeVirtualGoodsStorage.create() : VirtualGoodsStorage.create();

  /**
   * StoreInfo
   */
  var StoreInfo = Soomla.StoreInfo = Soomla.declareClass('StoreInfo', {
    KEY_META_STORE_INFO: 'meta.storeinfo',

    virtualItems: null,
    purchasableItems: null,
    goodsCategories: null,
    goodsUpgrades: null,
    currencies: null,
    currencyPacks: null,
    goods: null,
    categories: null,

    init: function (storeAssets) {

      Soomla.logDebug('Setting store assets in SoomlaInfo');

      if (!storeAssets) {
        Soomla.logDebug('The given store assets can\'t be null!');
        return false;
      }

      this.setStoreAssets(storeAssets);

      // At this point we have StoreInfo JSON saved at the local key-value storage. We can just
      // continue by initializing from DB.

      this.initializeFromDB();

      return true;
    },

    setStoreAssets: function (storeAssets) {
      var jsonString = JSON.stringify(storeAssets);
      Soomla.keyValueStorage.setValue(this.KEY_META_STORE_INFO, jsonString);
    },

    initializeFromDB: function () {
      var val = Soomla.keyValueStorage.getValue(this.KEY_META_STORE_INFO);

      if (!val) {
        var message = 'store json is not in DB. Make sure you initialized SoomlaStore with your Store assets. The App will shut down now.';
        Soomla.logError(message);
        throw message;
      }

      Soomla.logDebug('the metadata-economy json (from DB) is ' + val);

      var storeAssets = JSON.parse(val);

      this.currencies = _.collect(storeAssets.currencies, function (currency) {
        return Soomla.factory.create(currency);
      });
      this.currencyPacks = _.collect(storeAssets.currencyPacks, function (currencyPack) {
        return Soomla.factory.create(currencyPack);
      });
      if (storeAssets.goods) {
        this.goods = _.collect(_.union(
          storeAssets.goods.singleUse,
          storeAssets.goods.lifetime,
          storeAssets.goods.equippable,
          storeAssets.goods.goodPacks,
          storeAssets.goods.goodUpgrades
        ), function (good) {
          return Soomla.factory.create(good);
        });
      } else {
        this.goods = [];
      }
      this.categories = _.collect(storeAssets.categories, function (category) {
        return Soomla.factory.create(category);
      });


      this.virtualItems = _.transform(
        _.groupBy(
          _.union(this.currencies, this.currencyPacks, this.goods),
          'itemId'),
        function (result, value, key) {
          result[key] = value[0];
        }
      );

      this.purchasableItems = _.transform(
        _.groupBy(
          _.filter(_.union(this.currencyPacks, this.goods),
            function (vi) {
              return vi.purchasableItem && vi.purchasableItem.marketItem;
            }
          ),
          function (vi) {
            return vi.purchasableItem.marketItem.productId;
          }
        ),
        function (result, value, key) {
          result[key] = value[0];
        }
      );

      this.goodsUpgrades = _.groupBy(_.filter(this.goods, {className: 'UpgradeVG'}), 'good_itemId');

      var goodsCategories = this.goodsCategories = {};
      _.each(this.categories, function (category) {
        _.each(category.goods_itemIds, function (itemId) {
          goodsCategories[itemId] = category;
        });
      });
    },

    getItemByItemId: function (itemId) {
      Soomla.logDebug('Trying to fetch an item with itemId: ' + itemId);

      var item = this.virtualItems[itemId];
      if (!item) {
        Soomla.logError('Virtual item was not found when searching with itemId=' + itemId);
      }
      return item;
    },
    getPurchasableItemWithProductId: function (productId) {
      Soomla.logDebug('Trying to fetch a purchasable item with productId: ' + productId);

      var item = this.purchasableItems[productId];
      if (!item) {
        Soomla.logError('Virtual item was not found when searching with productId=' + productId);
      }
      return item;
    },
    getCategoryForVirtualGood: function (goodItemId) {
      Soomla.logDebug('Trying to fetch a category for a good with itemId: ' + goodItemId);

      var category = this.goodsCategories[goodItemId];
      if (!category) {
        Soomla.logError('Virtual item was not found when searching with goodItemId of category=' + goodItemId);
      }
      return category;
    },
    getFirstUpgradeForVirtualGood: function (goodItemId) {
      Soomla.logDebug('Trying to fetch first upgrade of a good with itemId: ' + goodItemId);

      var upgrades = this.goodsUpgrades[goodItemId];
      if (!upgrades) {
        return null;
      }

      return _.find(upgrades, function (upgradeVG) {
        return _.isUndefined(upgradeVG.prev_itemId) ||
          _.isNull(upgradeVG.prev_itemId) ||
          _.isEmpty(upgradeVG.prev_itemId);
      });
    },
    getLastUpgradeForVirtualGood: function (goodItemId) {
      Soomla.logDebug('Trying to fetch last upgrade of a good with itemId: ' + goodItemId);

      var upgrades = this.goodsUpgrades[goodItemId];
      if (!upgrades) {
        return null;
      }

      return _.find(upgrades, function (upgradeVG) {
        return _.isUndefined(upgradeVG.next_itemId) ||
          _.isNull(upgradeVG.next_itemId) ||
          _.isEmpty(upgradeVG.next_itemId);
      });
    },
    getUpgradesForVirtualGood: function (goodItemId) {
      Soomla.logDebug('Trying to fetch upgrades of a good with itemId: ' + goodItemId);

      return this.goodsUpgrades[goodItemId];
    },

    getVirtualItems: function () {
      return this.virtualItems;
    },

    getPurchasableItems: function () {
      return this.purchasableItems;
    },

    getGoodsCategories: function () {
      return this.goodsCategories;
    },

    getGoodsUpgrades: function () {
      return this.goodsUpgrades;
    },

    getCurrencies: function () {
      return this.currencies;
    },

    getCurrencyPacks: function () {
      return this.currencyPacks;
    },

    getGoods: function () {
      return this.goods;
    },

    getCategories: function () {
      return this.categories;
    },

    saveItem: function (virtualItem, saveToDB) {
      this.replaceVirtualItem(virtualItem);
      if (saveToDB) {
        this.save();
      }
    },
    saveItems: function (virtualItems, saveToDB) {
      if (!virtualItems || virtualItems.length == 0) {
        return;
      }

      _.each(virtualItems, function (vi) {
        this.replaceVirtualItem(vi);
      });

      if (saveToDB) {
        this.save();
      }
    },
    save: function () {
      var assets = Soomla.IStoreAssets.create();
      assets.currencies = this.currencies;
      assets.currencyPacks = this.currencyPacks;

      _.each(this.goods, function (vi) {
        if (vi.className === 'SingleUseVG') {
          assets.goods.singleUse.push(vi);
        } else if (vi.className === 'EquippableVG') {
          assets.goods.equippable.push(vi);
        } else if (vi.className === 'UpgradeVG') {
          assets.goods.goodUpgrades.push(vi);
        } else if (vi.className === 'LifetimeVG') {
          assets.goods.lifetime.push(vi);
        } else if (vi.className === 'SingleUsePackVG') {
          assets.goods.goodPacks.push(vi);
        }
      });
      assets.categories = this.categories;

      var jsonString = JSON.stringify(assets);
      Soomla.logDebug('saving StoreInfo to DB. json is: ' + jsonString);
      Soomla.keyValueStorage.setValue(this.KEY_META_STORE_INFO, jsonString);
    },

    replaceVirtualItem: function (virtualItem) {
      var foundIdx;
      this.virtualItems[virtualItem.itemId] = virtualItem;

      if (virtualItem.className === 'VirtualCurrency') {
        foundIdx = _.findIndex(this.currencies, {itemId: virtualItem.itemId});
        if (foundIdx >= 0) {
          _.slice(this.currencies, foundIdx);
        }
        this.currencies.push(virtualItem);
      }

      if (virtualItem.className === 'VirtualCurrencyPack') {
        if (virtualItem.purchasableItem && virtualItem.purchasableItem.marketItem) {
          this.purchasableItems[virtualItem.purchasableItem.marketItem.productId] = virtualItem;
        }

        foundIdx = _.findIndex(this.currencyPacks, {itemId: virtualItem.itemId});
        if (foundIdx >= 0) {
          _.slice(this.currencyPacks, foundIdx);
        }
        this.currencyPacks.push(virtualItem);
      }

      if (virtualItem.className === 'SingleUseVG' ||
        virtualItem.className === 'EquippableVG' ||
        virtualItem.className === 'UpgradeVG' ||
        virtualItem.className === 'LifetimeVG' ||
        virtualItem.className === 'SingleUsePackVG') {

        if (virtualItem.className === 'UpgradeVG') {
          foundIdx = _.findIndex(this.goodsUpgrades, {itemId: virtualItem.itemId});
          if (foundIdx >= 0) {
            _.slice(this.goodsUpgrades, foundIdx);
          }
          this.goodsUpgrades.push(virtualItem);
        }

        if (virtualItem.purchasableItem && virtualItem.purchasableItem.marketItem) {
          this.purchasableItems[virtualItem.purchasableItem.marketItem.productId] = virtualItem;
        }

        foundIdx = _.findIndex(this.goods, {itemId: virtualItem.itemId});
        if (foundIdx >= 0) {
          _.slice(this.goods, foundIdx);
        }
        this.goods.push(virtualItem);
      }
    }
  });

  var NativeStoreInfo = Soomla.NativeStoreInfo = Soomla.declareClass('NativeStoreInfo', {
    setStoreAssets: function setStoreAssets(storeAssets) {
      Soomla.logDebug('pushing CCStoreAssets to StoreInfo on native side');

      Soomla.callNative({
        method: 'CCStoreAssets::init',
        version: storeAssets.version,
        storeAssets: storeAssets
      });

      Soomla.logDebug('done! (pushing data to StoreAssets on native side)');
    },
    save: function save() {
      StoreInfo.save.apply(this, arguments);

      Soomla.callNative({
        method: 'CCStoreInfo::loadFromDB'
      });
    }
  }, StoreInfo);

  StoreInfo.createShared = function (storeAssets) {
    var ret = Soomla.platform.isNativeSupported() ? NativeStoreInfo.create() : StoreInfo.create();
    if (ret.init(storeAssets)) {
      Soomla.storeInfo = ret;
    } else {
      Soomla.storeInfo = null;
    }
  };

  var IStoreAssets = Soomla.IStoreAssets = Soomla.declareClass('IStoreAssets', {
    categories: [],
    currencies: [],
    currencyPacks: [],
    goods: {
      singleUse: [],
      lifetime: [],
      equippable: [],
      goodUpgrades: [],
      goodPacks: []
    },
    version: 1
  });

  /**
   * StoreEventHandler
   * Here for documentation reasons only, shows signatures for event handlers
   */
  var StoreEventHandler = Soomla.StoreEventHandler = function () {
    return Soomla.declareClass('StoreEventHandler', {
      onBillingNotSupported: function () {
      },
      onBillingSupported: function () {
      },
      onCurrencyBalanceChanged: function (virtualCurrency, balance, amountAdded) {
      },
      onGoodBalanceChanged: function (virtualGood, balance, amountAdded) {
      },
      onGoodEquipped: function (equippableVG) {
      },
      onGoodUnEquipped: function (equippableVG) {
      },
      onGoodUpgrade: function (virtualGood, upgradeVG) {
      },
      onItemPurchased: function (purchasableVirtualItem) {
      },
      onItemPurchaseStarted: function (purchasableVirtualItem) {
      },
      onMarketPurchaseCancelled: function (purchasableVirtualItem) {
      },
      onMarketPurchase: function (purchasableVirtualItem, token, payload) {
      },
      onMarketPurchaseStarted: function (purchasableVirtualItem) {
      },
      onMarketItemsRefreshStarted: function () {
      },
      onMarketItemsRefreshFailed: function (errorMessage) {
      },
      onMarketItemsRefreshed: function (marketItems) {
      },
      onMarketPurchaseVerification: function (purchasableVirtualItem) {
      },
      onRestoreTransactionsStarted: function () {
      },
      onRestoreTransactionsFinished: function (success) {
      },
      onUnexpectedErrorInStore: function () {
      },
      onSoomlaStoreInitialized: function () {
      },
      // For Android only
      onMarketRefund: function (purchasableVirtualItem) {
      },
      onIabServiceStarted: function () {
      },
      onIabServiceStopped: function () {
      }
    });
  }();

  var StoreEventDispatcher = Soomla.StoreEventDispatcher = function () {

    var StoreEventDispatcher = Soomla.declareClass('StoreEventDispatcher', {
      init: function () {
        var eventDispatcher = Soomla.soomlaEventDispatcher;
        eventDispatcher.registerEventHandler(StoreConsts.EVENT_BILLING_NOT_SUPPORTED, _.bind(function (parameters) {
          Soomla.fireSoomlaEvent(parameters.method);
        }, this));

        eventDispatcher.registerEventHandler(StoreConsts.EVENT_BILLING_SUPPORTED, _.bind(function (parameters) {
          Soomla.fireSoomlaEvent(parameters.method);
        }, this));

        eventDispatcher.registerEventHandler(StoreConsts.EVENT_CURRENCY_BALANCE_CHANGED, _.bind(function (parameters) {
          var virtualCurrency = Soomla.storeInfo.getItemByItemId(parameters.itemId);

          Soomla.storeInventory.refreshOnCurrencyBalanceChanged(virtualCurrency, parameters.balance, parameters.amountAdded);

          Soomla.fireSoomlaEvent(parameters.method, [virtualCurrency, parameters.balance, parameters.amountAdded]);
        }, this));

        eventDispatcher.registerEventHandler(StoreConsts.EVENT_GOOD_BALANCE_CHANGED, _.bind(function (parameters) {
          var virtualGood = Soomla.storeInfo.getItemByItemId(parameters.itemId);

          Soomla.storeInventory.refreshOnGoodBalanceChanged(virtualGood, parameters.balance, parameters.amountAdded);

          Soomla.fireSoomlaEvent(parameters.method, [virtualGood, parameters.balance, parameters.amountAdded]);
        }, this));

        eventDispatcher.registerEventHandler(StoreConsts.EVENT_GOOD_EQUIPPED, _.bind(function (parameters) {
          var equippableVG = Soomla.storeInfo.getItemByItemId(parameters.itemId);

          Soomla.storeInventory.refreshOnGoodEquipped(equippableVG);

          Soomla.fireSoomlaEvent(parameters.method, [equippableVG]);
        }, this));

        eventDispatcher.registerEventHandler(StoreConsts.EVENT_GOOD_UNEQUIPPED, _.bind(function (parameters) {
          var equippableVG = Soomla.storeInfo.getItemByItemId(parameters.itemId);

          Soomla.storeInventory.refreshOnGoodUnEquipped(equippableVG);

          Soomla.fireSoomlaEvent(parameters.method, [equippableVG]);
        }, this));

        eventDispatcher.registerEventHandler(StoreConsts.EVENT_GOOD_UPGRADE, _.bind(function (parameters) {
          var virtualGood = Soomla.storeInfo.getItemByItemId(parameters.itemId);
          var upgradeVG = Soomla.storeInfo.getItemByItemId(parameters.vguItemId);

          Soomla.storeInventory.refreshOnGoodUpgrade(virtualGood, upgradeVG);

          Soomla.fireSoomlaEvent(parameters.method, [virtualGood, upgradeVG]);
        }, this));

        eventDispatcher.registerEventHandler(StoreConsts.EVENT_ITEM_PURCHASED, _.bind(function (parameters) {
          var purchasableVirtualItem = Soomla.storeInfo.getItemByItemId(parameters.itemId);
          Soomla.fireSoomlaEvent(parameters.method, [purchasableVirtualItem]);
        }, this));

        eventDispatcher.registerEventHandler(StoreConsts.EVENT_ITEM_PURCHASE_STARTED, _.bind(function (parameters) {
          var purchasableVirtualItem = Soomla.storeInfo.getItemByItemId(parameters.itemId);
          Soomla.fireSoomlaEvent(parameters.method, [purchasableVirtualItem]);
        }, this));

        eventDispatcher.registerEventHandler(StoreConsts.EVENT_MARKET_PURCHASE_CANCELED, _.bind(function (parameters) {
          var purchasableVirtualItem = Soomla.storeInfo.getItemByItemId(parameters.itemId);
          Soomla.fireSoomlaEvent(parameters.method, [purchasableVirtualItem]);
        }, this));

        eventDispatcher.registerEventHandler(StoreConsts.EVENT_MARKET_PURCHASE, _.bind(function (parameters) {
          var purchasableVirtualItem = Soomla.storeInfo.getItemByItemId(parameters.itemId);
          var token = parameters.token;
          var payload = parameters.payload;
          Soomla.fireSoomlaEvent(parameters.method, [purchasableVirtualItem, token, payload]);
        }, this));

        eventDispatcher.registerEventHandler(StoreConsts.EVENT_MARKET_PURCHASE_STARTED, _.bind(function (parameters) {
          var purchasableVirtualItem = Soomla.storeInfo.getItemByItemId(parameters.itemId);
          Soomla.fireSoomlaEvent(parameters.method, [purchasableVirtualItem]);
        }, this));

        eventDispatcher.registerEventHandler(StoreConsts.EVENT_MARKET_ITEMS_REFRESHED, _.bind(function (parameters) {
          var marketItemsDict = parameters.marketItems;
          var marketItems = [];
          _.forEach(marketItemsDict, function (marketItem) {

            // be careful confusing naming: snake_case VS camelCase
            var productId = marketItem.productId;
            var marketPrice = marketItem.marketPrice;
            var marketTitle = marketItem.marketTitle;
            var marketDescription = marketItem.marketDesc;

            var pvi = Soomla.storeInfo.getPurchasableItemWithProductId(productId);

            var purchaseWithMarket = pvi.purchasableItem;
            var mi = purchaseWithMarket.marketItem;

            mi.marketPrice = marketPrice;
            mi.marketTitle = marketTitle;
            mi.marketDescription = marketDescription;

            marketItems.push(pvi);
          });

          Soomla.fireSoomlaEvent(parameters.method, [marketItems]);
        }, this));

        eventDispatcher.registerEventHandler(StoreConsts.EVENT_MARKET_ITEMS_REFRESH_STARTED, _.bind(function (parameters) {
          Soomla.fireSoomlaEvent(parameters.method);
        }, this));

        eventDispatcher.registerEventHandler(StoreConsts.EVENT_MARKET_ITEMS_REFRESH_FAILED, _.bind(function (parameters) {
          Soomla.fireSoomlaEvent(parameters.method, [parameters.errorMessage]);
        }, this));

        eventDispatcher.registerEventHandler(StoreConsts.EVENT_MARKET_PURCHASE_VERIFICATION, _.bind(function (parameters) {
          var purchasableVirtualItem = Soomla.storeInfo.getItemByItemId(parameters.itemId);
          Soomla.fireSoomlaEvent(parameters.method, [purchasableVirtualItem]);
        }, this));

        eventDispatcher.registerEventHandler(StoreConsts.EVENT_RESTORE_TRANSACTION_FINISHED, _.bind(function (parameters) {
          Soomla.fireSoomlaEvent(parameters.method, [parameters.success]);
        }, this));

        eventDispatcher.registerEventHandler(StoreConsts.EVENT_RESTORE_TRANSACTION_STARTED, _.bind(function (parameters) {
          Soomla.fireSoomlaEvent(parameters.method);
        }, this));

        eventDispatcher.registerEventHandler(StoreConsts.EVENT_UNEXPECTED_ERROR_IN_STORE, _.bind(function (parameters) {
          Soomla.fireSoomlaEvent(parameters.method);
        }, this));

        eventDispatcher.registerEventHandler(StoreConsts.EVENT_SOOMLA_STORE_INITIALIZED, _.bind(function (parameters) {
          Soomla.fireSoomlaEvent(parameters.method);
        }, this));

        //  Android specific
        eventDispatcher.registerEventHandler(StoreConsts.EVENT_MARKET_REFUND, _.bind(function (parameters) {
          var purchasableVirtualItem = Soomla.storeInfo.getItemByItemId(parameters.itemId);
          Soomla.fireSoomlaEvent(parameters.method, [purchasableVirtualItem]);
        }, this));

        eventDispatcher.registerEventHandler(StoreConsts.EVENT_IAB_SERVICE_STARTED, _.bind(function (parameters) {
          Soomla.fireSoomlaEvent(parameters.method);
        }, this));

        eventDispatcher.registerEventHandler(StoreConsts.EVENT_IAB_SERVICE_STOPPED, _.bind(function (parameters) {
          Soomla.fireSoomlaEvent(parameters.method);
        }, this));

        return true;
      }
    });

    StoreEventDispatcher.initShared = function () {
      var ret = StoreEventDispatcher.create();
      if (ret.init()) {
        Soomla.storeEventDispatcher = ret;
      } else {
        Soomla.storeEventDispatcher = null;
      }
    };

    return StoreEventDispatcher;
  }();

  var StoreBridge = Soomla.StoreBridge = Soomla.declareClass('StoreBridge', {
    init: function () {
      return true;
    },
    applyParams: function applyParams(storeParams) {
    }
  });

  var NativeStoreBridge = Soomla.NativeStoreBridge = Soomla.declareClass('NativeStoreBridge', {
    init: function () {
      this.bindNative();
      return true;
    },
    applyParams: function applyParams(storeParams) {
      if (Soomla.platform.isIos()) {
        Soomla.callNative({
          method: 'CCSoomlaStore::setSSV',
          ssv: storeParams.SSV
        });
      }

      if (Soomla.platform.isAndroid()) {
        Soomla.callNative({
          method: 'CCSoomlaStore::setAndroidPublicKey',
          androidPublicKey: storeParams.androidPublicKey
        });
        Soomla.callNative({
          method: 'CCSoomlaStore::setTestPurchases',
          testPurchases: storeParams.testPurchases
        });
      }
    },

    bindNative: function bindNative() {
      Soomla.logDebug('Binding to native platform Store bridge...');

      if (Soomla.platform.isAndroid()) {
        jsb.reflection.callStaticMethod('com/soomla/cocos2dx/store/StoreBridgeBinder', 'bind', '()V');
      } else if (Soomla.platform.isIos()) {
        jsb.reflection.callStaticMethod('StoreBridge', 'initShared');
      } else {
        Soomla.logError('Unsupported platform: ' + Soomla.platform.name);
      }
    }
  }, StoreBridge);

  StoreBridge.initShared = function () {
    var ret = Soomla.platform.isNativeSupported() ? NativeStoreBridge.create() : StoreBridge.create();
    if (ret.init()) {
      Soomla.storeBridge = ret;
    } else {
      Soomla.storeBridge = null;
    }
  };

  /**
   * SoomlaStore
   */
  var SoomlaStore = Soomla.SoomlaStore = Soomla.declareClass('SoomlaStore', {
    SOOMLA_AND_PUB_KEY_DEFAULT: 'YOUR GOOGLE PLAY PUBLIC KEY',
    initialized: false,
    initialize: function (storeAssets, storeParams) {

      if (this.initialized) {
        var err = 'SoomlaStore is already initialized. You can\'t initialize it twice!';
        Soomla.fireSoomlaEvent(StoreConsts.EVENT_UNEXPECTED_ERROR_IN_STORE, [err, true]);
        Soomla.logError(err);
        return;
      }

      StoreEventDispatcher.initShared();
      StoreBridge.initShared();

      Soomla.logDebug('CCSoomlaStore Initializing...');

      this.loadBillingService();

      StoreInfo.createShared(storeAssets);

      Soomla.storeBridge.applyParams(storeParams);

      if (Soomla.platform.isIos()) {
        this.refreshMarketItemsDetails();
      } else if (Soomla.platform.isAndroid()) {
        this.refreshInventory();
      }

      this.initialized = true;
      Soomla.fireSoomlaEvent(StoreConsts.EVENT_SOOMLA_STORE_INITIALIZED, [true]);

      return true;
    },
    buyMarketItem: function (productId, payload) {
      ////===========
      var item = Soomla.storeInfo.getPurchasableItemWithProductId(productId);
      if (!item) {
        return;
      }

      // simulate onMarketPurchaseStarted event
      Soomla.fireSoomlaEvent(StoreConsts.EVENT_MARKET_PURCHASE_STARTED, [item]);

      // in the editor we just give the item... no real market.
      item.give(1);

      // simulate onMarketPurchase event
      Soomla.fireSoomlaEvent(StoreConsts.EVENT_MARKET_PURCHASE, [item, 'fake_token_zyxw9876', payload]);
    },
    restoreTransactions: function () {
    },
    refreshInventory: function () {
    },
    refreshMarketItemsDetails: function () {
    },
    // For iOS only
    transactionsAlreadyRestored: function () {
    },
    // For Android only
    startIabServiceInBg: function () {
    },
    // For Android only
    stopIabServiceInBg: function () {
    },

    loadBillingService: function loadBillingService() {

    }
  });

  /**
   * NativeSoomlaStore
   */
  var NativeSoomlaStore = Soomla.NativeSoomlaStore = Soomla.declareClass('NativeSoomlaStore', {
    buyMarketItem: function (productId, payload) {
      Soomla.callNative({
        method: 'CCSoomlaStore::buyMarketItem',
        productId: productId,
        payload: payload
      });
    },
    restoreTransactions: function () {
      Soomla.callNative({
        method: 'CCSoomlaStore::restoreTransactions'
      });
    },
    refreshInventory: function () {
      Soomla.callNative({
        method: 'CCSoomlaStore::refreshInventory'
      });
    },
    refreshMarketItemsDetails: function () {
      Soomla.callNative({
        method: 'CCSoomlaStore::refreshMarketItemsDetails'
      });
    },
    // For iOS only
    transactionsAlreadyRestored: function () {
      var retParams = Soomla.callNative({
        method: 'CCSoomlaStore::transactionsAlreadyRestored'
      });
      return retParams['return'];
    },
    // For Android only
    startIabServiceInBg: function () {
      Soomla.callNative({
        method: 'CCSoomlaStore::startIabServiceInBg'
      });
    },
    // For Android only
    stopIabServiceInBg: function () {
      Soomla.callNative({
        method: 'CCSoomlaStore::stopIabServiceInBg'
      });
    },

    loadBillingService: function () {
      Soomla.callNative({
        method: 'CCSoomlaStore::loadBillingService'
      });
    }
  }, SoomlaStore);

  Soomla.soomlaStore = Soomla.platform.isNativeSupported() ? NativeSoomlaStore.create() : SoomlaStore.create();

  var StoreInventory = Soomla.StoreInventory = Soomla.declareClass('StoreInventory', {
    mLocalItemBalances: {},
    mLocalUpgrades: {},
    mLocalEquippedGoods: [],
    init: function () {
      return true;
    },
    canAfford: function (itemId) {
      Soomla.logDebug('Checking can afford: ' + itemId);

      var pvi = Soomla.storeInfo.getItemByItemId(itemId);
      if (pvi) {
        pvi.canAfford();
      } else {
        return false;
      }
    },
    buyItem: function (itemId, payload) {
      Soomla.logDebug('Buying: ' + itemId);

      var pvi = Soomla.storeInfo.getItemByItemId(itemId);
      if (pvi) {
        pvi.buy(payload);
      }
    },
    getItemBalance: function (itemId) {
      var amount = this.mLocalItemBalances[itemId];
      if (!_.isUndefined(amount) && !_.isNull(amount)) {
        return amount;
      }

      var item = Soomla.storeInfo.getItemByItemId(itemId);
      if (!item) {
        return 0;
      }
      return item.getBalance();
    },
    giveItem: function (itemId, amount) {
      Soomla.logDebug('Giving: ' + amount + ' pieces of: ' + itemId);

      var item = Soomla.storeInfo.getItemByItemId(itemId);
      if (item) {
        item.give(amount);
      }
    },
    takeItem: function (itemId, amount) {
      Soomla.logDebug('Taking: ' + amount + ' pieces of: ' + itemId);

      var item = Soomla.storeInfo.getItemByItemId(itemId);
      if (item) {
        item.take(amount);
      }
    },
    equipVirtualGood: function (itemId) {
      Soomla.logDebug('Equipping: ' + itemId);

      var item = Soomla.storeInfo.getItemByItemId(itemId);
      if (item) {
        item.equip();
      }
    },
    unEquipVirtualGood: function (itemId) {
      Soomla.logDebug('UnEquipping: ' + itemId);

      var item = Soomla.storeInfo.getItemByItemId(itemId);
      if (item) {
        item.unequip();
      }
    },
    isVirtualGoodEquipped: function (itemId) {
      Soomla.logDebug('Checking if ' + itemId + ' is equipped');

      var item = Soomla.storeInfo.getItemByItemId(itemId);
      if (item) {
        return Soomla.virtualGoodsStorage.isEquipped(item);
      } else {
        return false;
      }
    },
    getGoodUpgradeLevel: function (goodItemId) {
      Soomla.logDebug('Checking ' + goodItemId + ' upgrade level');

      var good = Soomla.storeInfo.getItemByItemId(goodItemId);
      if (!good) {
        Soomla.logError('You tried to get the level of a non-existant virtual good.');
        return 0;
      }
      var upgradeVG = Soomla.virtualGoodsStorage.getCurrentUpgrade(good);
      if (!upgradeVG) {
        return 0; //no upgrade
      }

      var first = Soomla.storeInfo.getFirstUpgradeForVirtualGood(goodItemId);
      var level = 1;
      while (first.itemId !== upgradeVG.itemId) {
        first = Soomla.storeInfo.getItemByItemId(first.next_itemId);
        level++;
      }

      return level;
    },
    getGoodCurrentUpgrade: function (goodItemId) {
      Soomla.logDebug('Checking ' + goodItemId + ' current upgrade');

      var good = Soomla.storeInfo.getItemByItemId(goodItemId);
      if (!good) {
        return '';
      }

      var upgradeVG = Soomla.virtualGoodsStorage.getCurrentUpgrade(good);
      if (!upgradeVG) {
        return '';
      }
      return upgradeVG.itemId;
    },
    upgradeGood: function (goodItemId) {
      Soomla.logDebug('Upgrading Good with: ' + goodItemId);
      var good = Soomla.storeInfo.getItemByItemId(goodItemId);
      if (!good) {
        return;
      }

      var upgradeVG = Soomla.virtualGoodsStorage.getCurrentUpgrade(good);

      if (upgradeVG) {
        var nextItemId = upgradeVG.next_itemId;
        if (_.isUndefined(nextItemId) || _.isNull(nextItemId)) {
          return;
        }
        var vgu = Soomla.storeInfo.getItemByItemId(nextItemId);
        if (vgu) {
          vgu.buy('');
        }
      }
      else {
        var first = Soomla.storeInfo.getFirstUpgradeForVirtualGood(goodItemId);
        if (first) {
          first.buy('');
        }
      }
    },
    removeGoodUpgrades: function (goodItemId) {
      Soomla.logDebug('Removing Good Upgrades for: ' + goodItemId);

      var upgrades = Soomla.storeInfo.getUpgradesForVirtualGood(goodItemId);
      _.forEach(upgrades, function (upgrade) {
        Soomla.virtualGoodsStorage.remove(upgrade, 1, true);
      });

      var good = Soomla.storeInfo.getItemByItemId(goodItemId);
      Soomla.virtualGoodsStorage.removeUpgrades(good);
    },

    refreshLocalInventory: function refreshLocalInventory() {
      this.mLocalItemBalances = {};
      this.mLocalUpgrades = {};
      this.mLocalEquippedGoods = [];

      _.forEach(Soomla.storeInfo.getCurrencies(), function (item) {
        this.mLocalItemBalances[item.itemId] = Soomla.virtualCurrencyStorage.getBalance(item);
      }, this);

      _.forEach(Soomla.storeInfo.getGoods(), function (item) {
        var balance = Soomla.virtualGoodsStorage.getBalance(item);
        this.mLocalItemBalances[item.itemId] = balance;

        var upgrade = Soomla.virtualGoodsStorage.getCurrentUpgrade(item);
        if (upgrade) {
          var upgradeLevel = this.getGoodUpgradeLevel(item.itemId);

          var localUpgrade = {
            itemId: upgrade.itemId,
            level: upgradeLevel
          };
          this.mLocalUpgrades[item.itemId] = localUpgrade;
        }

        if (item.className === 'EquippableVG') {
          if (Soomla.virtualGoodsStorage.isEquipped(item)) {
            this.mLocalEquippedGoods.push(item);
          }
        }

      }, this);
    },

    refreshOnGoodUpgrade: function refreshOnGoodUpgrade(vg, uvg) {
      if (!uvg) {
        delete this.mLocalUpgrades[vg.itemId];
      } else {
        var upgradeLevel = this.getGoodUpgradeLevel(vg.itemId);
        var upgrade = this.mLocalUpgrades[vg.itemId];
        if (upgrade) {
          upgrade.item = uvg.itemId;
          upgrade.level = upgradeLevel;
        } else {
          var localUpgrade = {itemId: uvg.itemId, level: upgradeLevel};
          this.mLocalUpgrades[vg.itemId] = localUpgrade;
        }
      }
    },
    refreshOnGoodEquipped: function refreshOnGoodEquipped(equippable) {
      this.mLocalEquippedGoods.push(equippable.itemId);
    },
    refreshOnGoodUnEquipped: function (equippable) {
      _.pull(this.mLocalEquippedGoods, equippable.itemId);
    },
    refreshOnCurrencyBalanceChanged: function refreshOnCurrencyBalanceChanged(virtualCurrency, balance, amountAdded) {
      this.updateLocalBalance(virtualCurrency.itemId, balance);
    },
    refreshOnGoodBalanceChanged: function refreshOnGoodBalanceChanged(good, balance, amountAdded) {
      this.updateLocalBalance(good.itemId, balance);
    },
    updateLocalBalance: function updateLocalBalance(itemId, balance) {
      this.mLocalItemBalances[itemId] = balance;
    }
  });

  StoreInventory.createShared = function () {
    var ret = StoreInventory.create();
    if (ret.init()) {
      return ret;
    } else {
      return null;
    }
  };

  Soomla.storeInventory = StoreInventory.createShared();


})();

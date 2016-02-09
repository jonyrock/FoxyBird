#include "Managers/EconomicsManager.h"
#include <functional>
#include <unistd.h>
#include <thread>
#include <iostream>

using namespace std;


struct EconomicsManagerImpl : public EconomicsManager {
    bool init() override {
	return true;
    }
    void askPurchase(function<void(bool, string)> fun) {
	fun(false, "asdsa");
    }
    void askRestore(std::function<void(bool, std::string)> fun)  {
	fun(false, "asdsa");
    }
    bool hasBoughtItem() override {
	return false;
    }
};

EconomicsManager* EconomicsManager::getInstance() {
    static EconomicsManagerImpl instance;
    return &instance;
}

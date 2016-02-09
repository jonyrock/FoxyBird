#pragma once

#include "BasicManager.h"
#include "PlatformMacros.h"

#include <string>

PLATFORM_DEPENDENT
struct DataManager : public BasicManager {

    virtual bool init() override { return true; }

    static DataManager* getInstance();

    virtual int  get(const std::string& key, int defaultValue) = 0;
    virtual bool get(const std::string& key, bool defaultValue) = 0;
    virtual std::string get(
        const std::string& key, 
        const std::string& defaultValue
    ) = 0;
    virtual void put(const std::string& key, const std::string& value) = 0;
    virtual void put(const std::string& key, int value) = 0;
    virtual void put(const std::string& key, bool value) = 0;

};
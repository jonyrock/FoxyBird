#pragma once

#include "BasicManager.h"
#include <string>

class AnalyticsManager : public BasicManager {

public:
    static AnalyticsManager* getInstance();
    
    void setScene(const std::string& str);
    void sendEvent(const std::string& str);
    
private:
    virtual bool init() override;
    
};
#pragma once

class BasicManager {
public:
    virtual bool init() = 0;
    virtual inline void onAppPause() { }
    virtual inline void onAppResume() { }
    virtual inline ~BasicManager() { }
};



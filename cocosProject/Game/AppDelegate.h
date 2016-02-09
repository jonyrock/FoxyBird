#pragma once

#include <cocos2d.h>

#include <vector>
#include <functional>

class BasicManager;
class AppDelegate : private cocos2d::Application {

public:
    
    /* HANDLERS */
    bool applicationDidFinishLaunching();
    void applicationDidEnterBackground();
    void applicationWillEnterForeground();

private:

    /* ELEMENTS */
    std::vector<BasicManager*> _managers;
    
    /* INITS */
    bool init();
    bool initManagers();
    
    /* FUNS */
    void multiresolutionSupport();

    /* TOOLS */
    void forAllManagers(std::function<void(BasicManager* m)> f);
    
};

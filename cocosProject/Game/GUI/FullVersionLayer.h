#pragma once

#include <cocos2d.h>
#include <cocos/ui/UIButton.h>
#include <string>
#include "GUI.h"


// https://trello.com/c/ACIddDOp/137-dev-desing

class FullVersionLayer : public cocos2d::LayerColor {

public:
    CREATE_FUNC(FullVersionLayer);
    // TODO: set callback on purchase
    inline bool isInProcess() const { return _inProcess; }
    
private:
    /* ELEMENTS */
    cocos2d::ui::Button* _buyButton = nullptr;
    GUI::TextButton* _restoreButton = nullptr;
    
    /* STATE */
    bool _inProcess = false;
    
    /* DATA */
    
    /* INITS */
    bool init() override;
    void initBackWithClose();
    void initTitle();
    void initPurchase();
    void initRestore();
    void initOverlayListener();
    
    /* HANDLERS */
    void onPurchase();
    void onRestore();
    void onBeginOperation();
    void onGetResults(bool, std::string);
    
};

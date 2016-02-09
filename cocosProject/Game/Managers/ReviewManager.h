#pragma once

#include "BasicManager.h"

class ReviewManager : public BasicManager {
public:
    static ReviewManager* getInstance();
    virtual bool init();
    void triggerReview();
    void onYes();
    void onLater();
    void onNo();
    
protected:
    void showReview();
    
private:
    int _reviewCount = 0;
    bool _isTurnedOn;
    
};
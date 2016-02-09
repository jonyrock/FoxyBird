#pragma once 

#include "BasicScene.h"


// https://trello.com/c/xW3ksN6J/165-design-dev-play

class MainScene : public BasicScene {

public:
    CREATE_FUNC(MainScene);

private:
    /* INITS */
    bool init() override;
    void initBack();
    void initButtons();

};


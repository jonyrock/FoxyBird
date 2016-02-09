#pragma once

#include "BasicScene.h"

class LoadingScene : public BasicScene {

public:

    CREATE_FUNC(LoadingScene);
    virtual bool init() override;

};
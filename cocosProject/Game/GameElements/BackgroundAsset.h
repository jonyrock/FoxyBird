#pragma once


#include "Models/FieldModel.h"
#include <cocos2d.h>
#include <vector>


class BackgroudAsset : public cocos2d::Node {

public:
    static BackgroudAsset* create(const FieldModel* fieldModel);
    inline virtual ~BackgroudAsset() { }
    virtual void update(float delta) override;

private:
    
    /* STATE */
    cocos2d::Size _size;
    const FieldModel* _fieldModel;
    
    /* ELEMENTS */
    cocos2d::LayerGradient* _backGradLeft;
    cocos2d::LayerGradient* _backGradRight;
    
    std::vector<cocos2d::Sprite*> _mountainsSprites;
    std::vector<cocos2d::Sprite*> _mountainsSpritesLight;
    
    /* INITS */
    bool init(const FieldModel* _fieldModel);
    void initMountains();
    void initBackground();
    void initStars();
    void initMoon();
    void initShadows();
    
    /* DEFAULTS */
    inline BackgroudAsset() { }
    inline BackgroudAsset(const BackgroudAsset& orig);
    
};
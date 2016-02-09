#pragma once
#include <cocos2d.h>


class PlayerAsset : public cocos2d::Node {
public:
    CREATE_FUNC(PlayerAsset);

    void setRotationInRadians(float rotation);
    float getRotationInDegrees() const;
    
    void addRotation(float angle);
    float getRotation() const;
    
private:
    
    /* INITS */
    bool init() override;
    
    inline PlayerAsset() { }
    inline PlayerAsset(const PlayerAsset& orig);
    
    class MySprite3d;
    MySprite3d* _sprite = nullptr;
    
    
};
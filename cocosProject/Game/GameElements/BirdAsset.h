#pragma once

#include <cocos2d.h>

#include <functional>
#include <vector>

class BirdAsset : public cocos2d::Node {
public:
    
    CREATE_FUNC(BirdAsset);
    
    void setRotationInRadians(float rotation);
    float getRotationInDegrees() const;

    void addRotation(float angle);
    float getRotation() const;
    
    /* HANDLERS */
    void onNextStage();
    
    /* TOOLS */
    void blink(std::function<void()> callback);
    
private:

    /* INITS */
    virtual bool init() override;
    bool initSprites();
    
    inline BirdAsset() { }
    inline BirdAsset(const BirdAsset& orig);
    
    class MySprite3d;
    
    /* STATE */
    size_t _nextSpriteIndex = 0;
    
    /* ELEMENTS */
    MySprite3d* _activeSprite = nullptr;
    std::vector<MySprite3d*> _sprites;
    std::vector<std::pair<std::string, std::string>> _birdPathes;
    
        
    /* TOOLS */
    MySprite3d* getSprite(size_t index);
    
};




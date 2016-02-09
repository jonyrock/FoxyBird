#pragma once

#include "Models/FieldModel.h"

#include <cocos2d.h>
#include <vector>
#include <list>
#include <map>

class PlatformModel;
class FieldDrawer : public cocos2d::DrawNode {

public:
    static FieldDrawer* create(const FieldModel* fieldModel);
    
    void update(float dt) override;

private:
    
    struct EnemyTailElement { float health; cocos2d::Vec2 position; };
    
    const FieldModel* _fieldModel = nullptr;
    
    /* STATE */
    cocos2d::Vec2 _birdLastPos;
    std::list<EnemyTailElement> _enemyTail;
    
    /* ELEMENTS */
    cocos2d::Layer* _water;
    std::map<PlatformModel*, cocos2d::ClippingNode*> _platfromToActivationSprite;
    
    /* INITS */
    bool init(const FieldModel* fieldModel);
    void initActivationSprites();
    
    /* UPDATES */
    void updateEnemyTail(float dt);
    
    /* DRAWS */
    void draw_();
    void drawPlatform(PlatformModel* p);
    // TODO: there should basic for drawIceHeight and drawTopIceHeight function cuz it is almost the same
    void drawIceHeight(int i, cocos2d::Vec2& centerPoint, std::vector<cocos2d::Vec2>& points);
    void drawTopIceHeight(int i, std::vector<cocos2d::Vec2>& points);
    void drawPlatfromHat(PlatformModel* p, cocos2d::Vec2& center, std::vector<cocos2d::Vec2>& points);
    void drawMoves();
    void drawPath(const cocos2d::Vec2& from, const cocos2d::Vec2& to, bool isLast, const cocos2d::Color4F& fromColor, const cocos2d::Color4F& toColor);
    void drawEnemyTail();
    void drawRect(cocos2d::Vec2 points[4], cocos2d::Color4F color);
    void drawTopIce();
    
    /* DEFAULT */
    FieldDrawer() { }
    FieldDrawer(const FieldDrawer& orig) { }
    virtual ~FieldDrawer() { }
    
} ;
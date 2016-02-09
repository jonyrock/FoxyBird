#pragma once 

#include <cocos2d.h>
#include <set>

#include "FieldDrawer.h"

class FieldModel;
class FieldAsset : public cocos2d::Node {
public:
    static FieldAsset* create(const FieldModel* fieldModel);
    
    void setMovesVisible(bool v);
    
private:
    
    /* STATE */
    const FieldModel* _fieldModel;
    cocos2d::Size _size;
    bool _isDrawMoves = false;
    
    /* ELEMENTS */
    FieldDrawer* _drawer;
    cocos2d::LayerGradient* _water;
    cocos2d::LayerGradient* _topIceGradient;
    cocos2d::DrawNode* _errorPathDrawer;
    cocos2d::Sprite* _waterGradient;
    
    /* INITS */
    bool init(const FieldModel* fieldModel);
    void initWater();
    void initDrawer();
    void initErrorPathDrawer();
    void initTopIceGragient();
    
    /* UPDATES */
    void update(float dt) override;
    void updateTutorialPaths();
    
    /* TOOLS */
    void drawPath(const cocos2d::Vec2& from, const cocos2d::Vec2& to, 
	bool isLast, const cocos2d::Color4F& fromColor, 
	const cocos2d::Color4F& toColor);
    
};
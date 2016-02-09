#pragma once


#include <cocos2d.h>
#include <vector>


// TODO: physics body separated
class FieldDrawer;
class PlatformModel : public cocos2d::Node {

public:
    virtual bool init() override;
    CREATE_FUNC(PlatformModel);
    
    inline virtual ~PlatformModel() { }
    
    inline void setIsSelected(bool v) { _isSelected = v; }
    
    inline bool getIsSelected() const { return _isSelected; }
    
    bool isInside(const cocos2d::Vec2& pos) const;
    
    // TODO: rename to "setAnimationHealth"
    inline void setHealth(float h) { _health = h; }
    
    inline float getHeath() const { return _health; }
    
    inline void setIsWrong(bool wrong) { _isWrong = wrong; }
    
    inline bool getIsWrong() const { return _isWrong; }
    
    
private:
    
    /* STATE */
    std::vector<cocos2d::Vec2> _points;
    int _health = 0;
    bool _isSelected = false;
    bool _isWrong = false;
    
    /* INITS */
    void initPoints();
    void initPhysics();
    
    /* TOOLS */
    inline PlatformModel() { }
    inline PlatformModel(const PlatformModel& orig);
    
    // TODO: I don`t like it
    friend FieldDrawer;

};
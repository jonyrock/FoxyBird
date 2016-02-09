#include "FieldAsset.h"
#include "FieldDrawer.h"
#include "Models/FieldModel.h"
#include "Utils.h"


using namespace cocos2d;
using namespace std;


FieldAsset* FieldAsset::create(const FieldModel* fieldModel) {
    auto pRet = new FieldAsset();
    if (pRet && pRet->init(fieldModel)) { 
        pRet->autorelease(); 
        return pRet; 
    } else { 
        delete pRet; 
        pRet = NULL; 
        return NULL; 
    }
}

bool FieldAsset::init(const FieldModel* fieldModel) {
    if(!Node::init()) {
        return false;
    }
    _fieldModel = fieldModel;
    _size = Director::getInstance()->getVisibleSize();
    initWater();
    initDrawer();
    initErrorPathDrawer();
    initTopIceGragient();
    scheduleUpdate();
    return true;
}

void FieldAsset::initDrawer() {
    _drawer = FieldDrawer::create(_fieldModel);
    addChild(_drawer, 2);
}

void FieldAsset::initErrorPathDrawer() {
    _errorPathDrawer = DrawNode::create();
    addChild(_errorPathDrawer, 4);
}

void FieldAsset::initWater() {
    _water = LayerGradient::create(); // used as LayerColor
    _water->setAnchorPoint(Vec2(0,0));
    float a = Director::getInstance()->getContentScaleFactor();
    _waterGradient = Sprite::create("radgr.png", Rect(0, 130/a, 400/a, (400 - 130)/a));
    
    _waterGradient->setAnchorPoint(Vec2(0.5, 1));
    _waterGradient->setScale(5);
    addChild(_water, 1);
    _water->addChild(_waterGradient);
    _waterGradient->setPosition(Vec2(_fieldModel->size.width/2, _fieldModel->size.height));
}

void FieldAsset::initTopIceGragient() {
     // this colors changed in update() but we set opacity here
    _topIceGradient =  LayerGradient::create(
         Color4B(0,0,0,1), Color4B(0,0,0,255), Vec2(0, -1)
    );
    addChild(_topIceGradient, 3);
}

void FieldAsset::update(float) {
    // see GameScene::generateTopIcePoints()
    _water->setScaleY(_fieldModel->yScale);

    _topIceGradient->setContentSize(Size(_size.width, 10 * _fieldModel->yScale));
    // TODO: here I add 2 cuz otherwise blinking line appears
    _topIceGradient->setPosition(Vec2(0, (_size.height - 1) * _fieldModel->yScale));
    
    _topIceGradient->setStartColor(Color3B(_fieldModel->colorIceTop));
    _topIceGradient->setEndColor(Color3B(_fieldModel->colorIceTop));
    
    _water->setStartColor(Color3B(_fieldModel->colorWater1));
    _water->setEndColor(Color3B(_fieldModel->colorWater1));
    _waterGradient->setColor(Color3B(_fieldModel->colorWater2));
    
    // TODO: special state var for this case
    updateTutorialPaths();
}

void FieldAsset::updateTutorialPaths() {
    return;
    if(!_isDrawMoves) return;
    _errorPathDrawer->clear();
    int prevJ = _fieldModel->playerPos;
    // TODO: make it constant
    vector<Color4F> colors;
    // TODO: draw coors backwards
    colors.push_back(Color4F(Utils::colorFromString("#565e7b"))); 
    colors.push_back(Color4F(Utils::colorFromString("#b7d968")));
    colors.push_back(Color4F(Utils::colorFromString("#b476ad")));
    colors.push_back(Color4F(Utils::colorFromString("#df4645")));
    colors.push_back(Color4F(Utils::colorFromString("#fde47f")));
    int index = 0;
    for(auto it = _fieldModel->moves.begin(); it != _fieldModel->moves.end(); ++it) {
	int j = *it;
	Vec2 from = _fieldModel->platforms.at(prevJ)->getPosition();
	from.y *= _fieldModel->yScale;
	Vec2 to = _fieldModel->platforms.at(j)->getPosition();
	to.y *= _fieldModel->yScale;
	auto itTest = it;
	itTest++;
	bool isLast = itTest == _fieldModel->moves.end();
	drawPath(from, to, isLast, colors.at(index), colors.at(index+1));
	index++;
	prevJ = j;
    }
    
}

// TODO: remove duplicate
static Color4F mixColors(Color4F colora, Color4F colorb, float partA) {
    float r = colora.r * partA + colorb.r * (1 - partA);
    float g = colora.g * partA + colorb.g * (1 - partA);
    float b = colora.b * partA + colorb.b * (1 - partA);
    float a = colora.a * partA + colorb.a * (1 - partA);
    return Color4F(r,g,b,a);
}

static Color4F mixColors(Color4F colora, Color4F colorb) {
    return mixColors(colora, colorb, 0.5f);
}

void FieldAsset::drawPath(
    const Vec2& from, const Vec2& to, bool isLast, const Color4F& fromColor, 
    const Color4F& toColor
) {
    Vec2 control = (from + to)/2 + Vec2(0, from.distance(to) / 4);
    float t = 0;
    Vec2 lastVertex;
    Vec2 vertex;    
    float drawUpperLimit = 1;
    if(isLast) drawUpperLimit = _fieldModel->birdJumpProgress;
    while(t < drawUpperLimit) {
	// TODO: compute this value somewhere else
        float x = powf(1 - t, 2) * from.x + 2.0f * (1 - t) * t * control.x + t * t * to.x;
        float y = powf(1 - t, 2) * from.y + 2.0f * (1 - t) * t * control.y + t * t * to.y;
        vertex = Vec2(x, y);
	if(lastVertex.distance(vertex) >= 6) {
	    auto colorBorder = Color4F::GRAY;
	    auto colorInnder = mixColors(fromColor, toColor, 1-t);
	    _errorPathDrawer->drawDot(vertex, 2.5, colorBorder);
	    _errorPathDrawer->drawDot(vertex, 2, colorInnder);
	    lastVertex = vertex;
	}
        t += 0.005;
    }
    
}

void FieldAsset::setMovesVisible(bool v) {
    _isDrawMoves = v;
    _errorPathDrawer->clear();
}




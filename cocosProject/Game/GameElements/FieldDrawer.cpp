#include "FieldDrawer.h"
#include "Models/FieldModel.h"
#include "Utils.h"
#include <algorithm>


using namespace cocos2d;
using namespace std;


const Vec2 LIGHT_VECTOR(0, -1);


FieldDrawer* FieldDrawer::create(const FieldModel* fieldModel) {
    auto pRet = new FieldDrawer();
    if (pRet && pRet->init(fieldModel)) { pRet->autorelease(); return pRet; }
    else { delete pRet; pRet = NULL; return NULL; }
}

/* INITS */

bool FieldDrawer::init(const FieldModel* fieldModel) {
    if(!DrawNode::init()) return false;
    _fieldModel = fieldModel;
    _birdLastPos = _fieldModel->birdPosition;
    initActivationSprites();
    scheduleUpdate();
    return true;
}

void FieldDrawer::initActivationSprites() {
    for(auto p: _fieldModel->platforms) {
	auto sprite = Sprite::create("radgr.png");
	sprite->setColor(Color3B::RED);
	auto cl = ClippingNode::create();
	cl->setContentSize(sprite->getContentSize());
	auto clDr = DrawNode::create();
	clDr->drawDot(Vec2(0,0), 100, Color4F::WHITE);
	clDr->setCameraMask((unsigned short)CameraFlag::USER2);
	cl->setStencil(clDr);
	cl->setPosition(_fieldModel->size/2);
	cl->addChild(sprite, 1);
	addChild(cl, 2);
	cl->setVisible(false);
	_platfromToActivationSprite.insert(make_pair(p, cl));
    }
}

void FieldDrawer::update(float dt) {
    updateEnemyTail(dt);
    draw_();
}

/* UPDATES */

void FieldDrawer::updateEnemyTail(float dt) {
    _enemyTail.remove_if([dt](EnemyTailElement& e) {
	e.health -= 150 * dt;
	return e.health <= 0; 
    });
    if(!_fieldModel->birdJumping) {
	_birdLastPos = _fieldModel->birdPosition;
    }
    if(_birdLastPos.distance(_fieldModel->birdPosition) > 12) {
	EnemyTailElement e;
	e.health = 100;
	e.position = _birdLastPos = _fieldModel->birdPosition;
	_enemyTail.push_front(e);
    }
}

/* DRAWS */

// TODO: make a normal draw function
void FieldDrawer::draw_() {
    clear();
    drawTopIce();
    // sort platform and draw it
    vector<int> platfromIndecesByY;
    for(int i = 0; i < (int)_fieldModel->platforms.size(); i++) {
	platfromIndecesByY.push_back(i);
    }
    sort(platfromIndecesByY.begin(), platfromIndecesByY.end(), 
	[this](int i, int j) {
	    return _fieldModel->platforms.at(i)->getPosition().y > 
		   _fieldModel->platforms.at(j)->getPosition().y;
	}
    );
    for(auto i: platfromIndecesByY) drawPlatform(_fieldModel->platforms.at(i));
    if(!_enemyTail.empty()) drawEnemyTail();
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

void FieldDrawer::drawPlatform(PlatformModel* p) {
    vector<Vec2> points;
    int mostLeft = -1;
    int mostRight = -1;
    int mostTop = -1;
    int mostBottom = -1;
    for(int i = 0; i < (int)p->_points.size(); i++) {
	// because we need rotation
	Vec2 resPoint = p->convertToWorldSpace(p->_points.at(i)); 
	resPoint.y *= _fieldModel->yScale;
	points.push_back(resPoint);
	if(mostLeft == -1 || resPoint.x < points.at(mostLeft).x) 
	    mostLeft = i;
	if(mostRight == -1 || resPoint.x > points.at(mostRight).x) 
	    mostRight = i;
	if(mostTop == -1 || resPoint.y >+ points.at(mostTop).y)
	    mostTop = i;
	if(mostBottom == -1 || resPoint.y < points.at(mostBottom).y)
	    mostBottom = i;
    }
    
    Vec2 centerPoint = p->convertToWorldSpace(
	p->getPhysicsBody()->getFirstShape()->getCenter()
    );
    centerPoint.y *= _fieldModel->yScale;
    
    /* DRAW NEAR HEIGHT */
    // there is a problem: we need to draw right part right to center
    // and left part left to center
    vector<int> platfromsToDraw;
    for(int i = mostRight; i != mostLeft; i = (i + 1) % points.size())
	platfromsToDraw.emplace_back(i);
    
    auto dr = [this, &platfromsToDraw, &p, &centerPoint, &points](int i) {
	drawIceHeight(platfromsToDraw.at(i), centerPoint, points);
    };
    int middleI = 0;
    for(; middleI + 1 < (int)platfromsToDraw.size(); middleI++) {
	float currentY = points.at(platfromsToDraw.at(middleI)).y;
	float nextY = points.at(platfromsToDraw.at(middleI+1)).y;
	// it it is true than it is the center
	if(currentY <= nextY) break;
	dr(middleI);
    }
    for(int i = (int)platfromsToDraw.size() - 1; i != middleI; i--) dr(i);
    dr(middleI);
    
    /* DRAW HEAD */
    drawPlatfromHat(p, centerPoint, points);
    
}

void FieldDrawer::drawIceHeight(int i, Vec2& center, std::vector<cocos2d::Vec2>& points) {
    float bottomHeight = 15 * _fieldModel->partToTargetScale();
    float topPart = 0.57;
    Vec2 topOffset(0,-0);
    if(bottomHeight < 2) return;
    vector<Vec2> pnts;
    Vec2 pointsTopRight = points.at((i + points.size()) % points.size());
    Vec2 pointsTopLeft = points.at((i + 1 + points.size()) % points.size());
    Vec2 dir = pointsTopLeft - pointsTopRight;
    dir.normalize();
    float colorPart = min(0.9, 0.4 + (LIGHT_VECTOR.dot(dir) + 1) / 2);
    Color4F lightColor(colorPart, colorPart, colorPart, 1);

    Vec2 pointRightToCenter1 = pointsTopRight + (center - pointsTopRight) / 15;
    Vec2 pointLeftToCenter1 = pointsTopLeft + (center - pointsTopLeft) / 15;

    pnts.push_back(topOffset + pointsTopRight);
    pnts.push_back(topOffset + pointRightToCenter1 + Vec2(0, -(bottomHeight * topPart)));
    pnts.push_back(topOffset + pointLeftToCenter1 + Vec2(0, -(bottomHeight * topPart)));
    pnts.push_back(topOffset + pointsTopLeft);
    drawRect(pnts.data(), lightColor);

    Vec2 pointRightToCenter2 = pointsTopRight + (center - pointsTopRight) / bottomHeight;
    Vec2 pointLeftToCenter2 = pointsTopLeft + (center - pointsTopLeft) / bottomHeight;

    pnts.clear();
    pnts.push_back(topOffset + pointRightToCenter1 + Vec2(0, -(bottomHeight * topPart)));
    pnts.push_back(topOffset + pointRightToCenter2 + Vec2(0, -bottomHeight));
    pnts.push_back(topOffset + pointLeftToCenter2 + Vec2(0, -bottomHeight));
    pnts.push_back(topOffset + pointLeftToCenter1 + Vec2(0, -(bottomHeight * topPart)));
    
    //const Color4F waterPseudeColor(Utils::colorFromString("#0055bf"));
    drawRect(pnts.data(), mixColors(lightColor, _fieldModel->colorSky1));
}   

void FieldDrawer::drawTopIceHeight(int i, std::vector<cocos2d::Vec2>& points) {
    // see GameScene::generateTopIcePoints()
    float bottomHeight = 15 * _fieldModel->partToTargetScale();
    float topPart = 0.57;
    Vec2 topOffset(0,-0);
    if(bottomHeight < 1) return;
    
    vector<Vec2> pnts;
    Vec2 pointsTopRight = points.at((i + points.size()) % points.size());
    Vec2 pointsTopLeft = points.at((i + 1 + points.size()) % points.size());
    Vec2 dir = pointsTopRight -pointsTopLeft;
    dir.normalize();
    float colorPart = min(0.9, 0.3 + (LIGHT_VECTOR.dot(dir) + 1) / 2.1);
    const Color4F iceColor = _fieldModel->colorIceTop;
    Color4F lightColor(iceColor.r * colorPart, iceColor.g * colorPart, iceColor.b * colorPart, 1);

    Vec2 pointRightToCenter1 = pointsTopRight;
    Vec2 pointLeftToCenter1 = pointsTopLeft;

    pnts.push_back(topOffset + pointsTopRight);
    pnts.push_back(topOffset + pointRightToCenter1 + Vec2(0, -(bottomHeight * topPart)));
    pnts.push_back(topOffset + pointLeftToCenter1 + Vec2(0, -(bottomHeight * topPart)));
    pnts.push_back(topOffset + pointsTopLeft);
    
    drawRect(pnts.data(), lightColor);
    
    // RETURN;
    return;

    Vec2 pointRightToCenter2 = pointsTopRight;
    Vec2 pointLeftToCenter2 = pointsTopLeft;

    pnts.clear();
    pnts.push_back(topOffset + pointRightToCenter1 + Vec2(0, -(bottomHeight * topPart)));
    pnts.push_back(topOffset + pointRightToCenter2 + Vec2(0, -bottomHeight));
    pnts.push_back(topOffset + pointLeftToCenter2 + Vec2(0, -bottomHeight));
    pnts.push_back(topOffset + pointLeftToCenter1 + Vec2(0, -(bottomHeight * topPart)));
    
    
    drawRect(pnts.data(), mixColors(lightColor, _fieldModel->colorSky1, 0.4));
    
}

void FieldDrawer::drawPlatfromHat(PlatformModel* p, Vec2& center, vector<Vec2>& points) {
    drawPolygon(points.data(), (int)points.size(), Color4F::WHITE, 0.2, Color4F(1,1,1,0.5));
    drawPolygon(points.data(), (int)points.size(), Color4F::WHITE, 0.3, Color4F(1,1,1,0.5));
//    auto cl = _platfromToActivationSprite.at(p);
//    auto dn = static_cast<DrawNode*>(cl->getStencil());
//    auto sp = static_cast<Sprite*>(cl->getChildren().at(0));
//    for(auto& pn: points) pn -= center;
    //cl->setVisible(false); 
    if(p->getHeath() > 1) {
//	dn->clear();
//	dn->drawPolygon(points.data(), points.size(), Color4F::WHITE, 0, Color4F::WHITE);
//	sp->setColor(Color3B::YELLOW);
//	if(p->getIsSelected()) {
//	    sp->setColor(p->getIsWrong()?Color3B::RED:Color3B::GREEN);
//	}
//	auto targetScale = 0.9 * p->getHeath() / 100;
//	sp->setScaleY(targetScale * _fieldModel->yScale);
//	sp->setScaleX(targetScale);
//	cl->setPosition(center);
//	cl->setVisible(true);
	auto color = Color4F::YELLOW;
	if(p->getIsSelected()) {
	    color = p->getIsWrong()?Color4F::RED:Color4F::GREEN;
        }
	color = mixColors(color, Color4F::WHITE, p->getHeath() / 100);
	drawPolygon(points.data(), (int)points.size(), color, 0.3, Color4F(1,1,1,0.5));
    } else { 
	//cl->setVisible(false); 
    }
}

void FieldDrawer::drawRect(cocos2d::Vec2 points[4], cocos2d::Color4F color) {
    Color4F borderColor(color);
    borderColor.a = 0.5;
    drawPolygon(points, 4, color, 0, borderColor);
}

void FieldDrawer::drawEnemyTail() {
    for(auto e: _enemyTail) {
	auto pos = e.position;
	auto color = Color4F::YELLOW;
	color.a = (e.health / 100) * 0.7;
	drawDot(pos, 4*e.health/100, color);
    }
}

void FieldDrawer::drawTopIce() {
    vector<Vec2> pnts;
    pnts.resize(4);
    for(size_t i = 0; i + 1 < _fieldModel->topIcePoints.size(); i++) {
	pnts.at(0) = _fieldModel->topIcePoints.at(i);
	pnts.at(1) = _fieldModel->topIcePoints.at(i + 1);
	pnts.at(2).x = _fieldModel->topIcePoints.at(i + 1).x;
	pnts.at(2).y = _fieldModel->size.height;
	pnts.at(3).x = _fieldModel->topIcePoints.at(i).x;
	pnts.at(3).y = _fieldModel->size.height;
	for(auto& p : pnts) p.y *= _fieldModel->yScale;
	drawTopIceHeight(0, pnts);
	const Color4F hatColor = _fieldModel->colorIceTop;
	const Color4F hatBorderColor = Color4F(hatColor.r, hatColor.g, hatColor.b, 0.5);
	drawPolygon(pnts.data(), (int)pnts.size(), hatColor, 0.2, hatBorderColor);
    }
}
#include "PlatformModel.h"
#include "Utils.h"

#include <cocos2d.h>

#include <vector>
#include <random>
#include <algorithm>
#include <cassert>
#include <math.h>
#include <map>


using namespace cocos2d;
using namespace std;


const int MAX_RADIUS = 57;
const int MIN_RADIUS = 42;


enum class Turn {
    left = 1,
    right = 2,
    zero = 4
};

// TODO: get turn and cw is the same
Turn getTurn(Vec2 a, Vec2 b, Vec2 c) {
    int det = a.x * (b.y - c.y) - a.y * (b.x - c.x) + (b.x * c.y - b.y * c.x);
    if (det > 0)
	return Turn::left;
    if (det < 0)
	return Turn::right;
    return Turn::zero;
}

bool PlatformModel::init() {
    if(!Node::init())
	return false;
    initPoints();
    initPhysics();
    return true;
}

bool cw(const Vec2& a, const Vec2& b, const Vec2& c) {
    return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x) < 0;
}

void PlatformModel::initPoints() {
    int pointsCount = 5 + rand() % 10;
    vector<Vec2> candidates;
    float angleStep = -2 * 3.1415 / pointsCount;
    float angleOffset = (angleStep / 100) * (rand() % 100);
    for(int i = 0; i < pointsCount; i++) {
	float part = (rand() % 20) * 1.0 / 20;
	Vec2 res(MIN_RADIUS + (MAX_RADIUS - MIN_RADIUS) * part, 0);
	float angle = i * angleStep;
	angle += angleStep * ((rand() % 18) - 9) * 0.05;
	res.rotate(Vec2(), angle + angleOffset);
	assert(res.length() <= MAX_RADIUS);
	candidates.push_back(res);
    }
    // convex hull
    int n = (int)candidates.size();
    int k = 0;
    sort(candidates.begin(), candidates.end());
    _points.resize(n * 2);
    for (int i = 0; i < n; _points[k++] = candidates[i++])
        for (; k >= 2 && !cw(_points[k - 2], _points[k - 1], candidates[i]); --k);
    for (int i = n - 2, t = k; i >= 0; _points[k++] = candidates[i--])
        for (; k > t && !cw(_points[k - 2], _points[k - 1], candidates[i]); --k);
    _points.resize(k - 1 - (_points[0] == _points[1]));
    
    // offset to the real center
    auto pb = PhysicsBody::createPolygon(_points.data(), (int)_points.size(),
	PhysicsMaterial(0.5, 0.5, 0));
    Vec2 geomCenter = pb->getShapes().at(0)->getCenter();
    
    Vec2 centerDiff = getPosition() - geomCenter;
    
    for(auto& p: _points) p += centerDiff;
    
}

void PlatformModel::initPhysics() {
    this->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    auto pb = PhysicsBody::createPolygon(_points.data(), (int)_points.size(),
	PhysicsMaterial(0.5, 0.5, 0));
    pb->setGravityEnable(true);
    pb->setContactTestBitmask(0xFFFFFFFF);
    setPhysicsBody(pb);
}
    
bool PlatformModel::isInside(const Vec2& pos_) const {
    
    // There is an O(nlog(n)) optimization but it will not be faster

    auto pos = convertToNodeSpace(pos_);
    
    // we are outer of circle
    if(pos.length() > MAX_RADIUS) return false;
    
    for(size_t i = 0; i < _points.size() ; i++) {
	Vec2 pointCurrent = _points.at(i);
	Vec2 pointNext = _points.at((i + 1) % _points.size());
	Turn turnNow = getTurn(Vec2(), pointCurrent, pos);
	Turn turnAfter = getTurn(Vec2(), pointNext, pos);
	// we are on an intersection
	if(turnNow == Turn::zero) return pos.length() <= _points.at(i).length();
	// we are in a middle
	if(turnNow == Turn::right && turnAfter == Turn::left) {
	    Turn myTurn = getTurn(pointCurrent, pointNext, pos);
	    return myTurn != Turn::left;
	}
    }
    return false;
    
}




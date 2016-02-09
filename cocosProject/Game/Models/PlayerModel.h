#pragma once

struct PlayerModel {
    bool    jumping	= false;
    bool    respawning	= false;
    float   speed	= -1;
    int	    targetPos;
    int	    sourcePos;
    float   fromRotation;
    float   jumpingTime;
    float   prevRotation;
    bool    isRotated;
    float   timeToRotate;
    float   rotationTime;
    float   rotationArc;
    float   pseudoHight = 0;
    float   health;
    inline PlayerModel() : 
	health(60)
    {}
};
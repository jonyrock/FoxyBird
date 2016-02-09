#pragma once

struct BirdModel {
    bool    jumping	= false;
    float   speed	= -1;
    int	    targetPos;	
    int	    sourcePos;
    int     stepsRemain; // TODO: rename to stageInitSteps
    float   rotationArc;
    float   fromRotation;
    float   pseudoHight = 0;
    int     moveQueueCount = 0;
};
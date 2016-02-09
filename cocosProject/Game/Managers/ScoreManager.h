#pragma once

#include "BasicManager.h"
#include <string>


class ScoreManager : public BasicManager  {
public:
    static ScoreManager* getInstance();    
    /*!
      * \brief update result: save it in the database
      * \return true if it's new best
      */
    bool sendMyScore(int v);
    int getBestScore();
private:
    bool init() override;
    

};
#pragma once

#include "BasicManager.h"

#include <string>
#include <chrono>

class LivesManager : public BasicManager  {
    
public:
    static LivesManager* getInstance();
    bool init() override;
    int  getLivesAvailable();
    int  getLivesTookCurrentSession() const;
    bool isFull();
    bool isUnlimited() const;
    bool isEmpty();
    void takeLife();
    void getStringTime(std::string& str);
    void askAboutNotificatoins();
    
    
protected:
    LivesManager();
    
    /* STATE */
    int _livesTookCurrentSession = 0;
    // never update it manually. use setLivesAvailable()
    int _livesAvailable;
    // never update it manually. use setTimePointToNextLife()
    std::chrono::time_point<std::chrono::system_clock> _timePointToNextLife;
    
    
    /* HANDLERS */
    void onTopLifeTake();
    
    /* TOOLS */
    void setTimePointToNextLife(std::chrono::time_point<std::chrono::system_clock> v);
    void setLivesAvailable(int v);
    void recount();
    void loadData();
    
    
};
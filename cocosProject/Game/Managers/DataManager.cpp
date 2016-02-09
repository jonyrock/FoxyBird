
#if (CC_TARGET_PLATFORM != CC_PLATFORM_ANDROID)

#include "DataManager.h"

#include <cocos2d.h>

#include <sqlite3.h>

#include <cassert>
#include <fstream>
#include <iostream>


#define DATABASE_FILE_NAME "data.db"


using namespace std;
using namespace cocos2d;


static bool isFileExist(const string& path) {
    return static_cast<bool>(ifstream(path));
}

struct DataManagerImpl : public DataManager {
    
    // TOOD: check thread saveness
    
    bool init() override {
        // TODO: copy new base if it is not exist in the writeable path
        auto path = FileUtils::getInstance()->getWritablePath() + DATABASE_FILE_NAME;
        if(!isFileExist(path)) {
            string pathInResources = FileUtils::getInstance()
                                        ->fullPathForFilename(DATABASE_FILE_NAME);
            CCLOG(
                "Replicate database from resource %s to %s", 
                pathInResources.c_str(), path.c_str()
            );
            ifstream source(pathInResources, ios::binary);
            ofstream dest(path, ios::binary);
            dest << source.rdbuf();
            source.close();
            dest.close();
        }
        
        int res = sqlite3_open(path.c_str(), &_ppDb);
        if(res != SQLITE_OK) {
            return false;
        }
        auto sqlPrep = [this](const char* query, sqlite3_stmt** stm) {
            sqlite3_prepare_v2(_ppDb, query, -1, stm, nullptr);
        };
        // init queries
        sqlPrep("select value from strings where key = ?;", &_stmtGetString);
        sqlPrep("select value from ints where key = ?;", &_stmtGetInt);
        sqlPrep("select value from bools where key = ?;", &_stmtGetBool);
        sqlPrep("update strings set value=? where key=?;", &_stmtSetString);
        sqlPrep("update ints set value=? where key=?;", &_stmtSetInt);
        sqlPrep("update bools set value=? where key=?;", &_stmtSetBool);
        return true;
    }
    
    int get(const string& key, int defaultValue) override {
        sqlite3_reset(_stmtGetInt);
        sqlite3_bind_text(_stmtGetInt, 1, key.c_str(), -1, SQLITE_TRANSIENT);
        if(SQLITE_ROW != sqlite3_step(_stmtGetInt)) {
            return defaultValue;
        }
        return sqlite3_column_int(_stmtGetInt, 0);
    }
    
    bool get(const string& key, bool defaultValue) override {
        sqlite3_reset(_stmtGetBool);
        sqlite3_bind_text(_stmtGetBool, 1, key.c_str(), -1, SQLITE_TRANSIENT);
        if(SQLITE_ROW != sqlite3_step(_stmtGetBool)) {
            return defaultValue;
        }
        return sqlite3_column_int(_stmtGetBool, 0) == 1;
    }
    
    string get(const string& key, const string& defaultValue) override {
	sqlite3_reset(_stmtGetString);
        sqlite3_bind_text(_stmtGetString, 1, key.c_str(), -1, SQLITE_TRANSIENT);
        if(SQLITE_ROW != sqlite3_step(_stmtGetString)) {
            return defaultValue;
        }
        const unsigned char * res = sqlite3_column_text(_stmtGetString, 0);
        return reinterpret_cast<const char*>(res);
    }
    
    void put(const string& key, int value) override {
	sqlite3_reset(_stmtSetInt);
        sqlite3_bind_int(_stmtSetInt, 1, value);
        sqlite3_bind_text(_stmtSetInt, 2, key.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_step(_stmtSetInt);
    }
    
    void put(const string& key, bool value) override {
        sqlite3_reset(_stmtSetBool);
        sqlite3_bind_int(_stmtSetBool, 1, static_cast<int>(value));
        sqlite3_bind_text(_stmtSetBool, 2, key.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_step(_stmtSetBool);
    }
    
    void put(const string& key, const string& value) override {
	sqlite3_reset(_stmtSetString);
        sqlite3_bind_text(_stmtSetString, 1, value.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(_stmtSetString, 2, key.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_step(_stmtSetString);
    }

   ~DataManagerImpl() {
        sqlite3_finalize(_stmtGetString);
        sqlite3_finalize(_stmtGetInt);
        sqlite3_finalize(_stmtGetBool);
        sqlite3_finalize(_stmtSetString);
        sqlite3_finalize(_stmtSetInt);
        sqlite3_finalize(_stmtSetBool);
    }
    
private:
    sqlite3* _ppDb;
    
    /* STATEMENTS */
    sqlite3_stmt* _stmtGetString;
    sqlite3_stmt* _stmtGetInt;
    sqlite3_stmt* _stmtGetBool;
    sqlite3_stmt* _stmtSetString;
    sqlite3_stmt* _stmtSetInt; 
    sqlite3_stmt* _stmtSetBool;

};

DataManager* DataManager::getInstance() {    
    static DataManagerImpl instance;
    return &instance;
}

#endif
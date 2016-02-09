#include "Managers/DataManager.h"

#include "leveldb/db.h"

#include <cocos2d.h>

#include <assert.h>


using namespace std;
using namespace cocos2d;


struct DataManagerImpl : public DataManager {
    
    leveldb::DB* _db = nullptr;

    DataManagerImpl() {
        string path = FileUtils::getInstance()->getWritablePath() + "userData.db";
        leveldb::Options options;
        options.create_if_missing = true;
        leveldb::Status status = leveldb::DB::Open(options, path, &_db);
        assert(status.ok());
    }
    
    bool init() override { return true; }

    int get(const string& key, int defaultValue) override {
        std::string value;
        leveldb::Status s = _db->Get(leveldb::ReadOptions(), key, &value);

        if(value == "") {
    	   put(key, defaultValue);
    	   return defaultValue;
        }

        stringstream ss(value);
        int valueInt;
        ss >> valueInt;
        return valueInt;

    }

    void put(const string& key, int value) override {
        stringstream ss;
        ss << value;
        _db->Put(leveldb::WriteOptions(), key, ss.str());
    }

    string get(const string& key, const string& defaultValue) override {
        std::string value;
        leveldb::Status s = _db->Get(leveldb::ReadOptions(), key, &value);
        if(value == ""){
    	   put(key, defaultValue);
    	   return defaultValue;
        }
        return value;
    }

    bool get(const string& key, bool defaultValue) override {
        std::string value;
        leveldb::Status s = _db->Get(leveldb::ReadOptions(), key, &value);
        if(value == "") {
    	   put(key, defaultValue ? 1 : 0);
    	   return defaultValue;
        }
        stringstream ss(value);
        int v;
        ss >> v;
        return v == 1 ? true : false;
    }

    void put(const std::string& key, bool value) override { 
        put(key, (int)value); 
    }

    void put(const std::string& key, const std::string& value) override {
        _db->Put(leveldb::WriteOptions(), key, value);
    }

    ~DataManagerImpl() {
        delete _db;
    }

};

DataManager* DataManager::getInstance() {
    static DataManagerImpl instance;
    return &instance;
}

















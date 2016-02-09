#include "Managers/LocalizationManager.h"
#include "Managers/DataManager.h"
#include "Managers/SettingsManager.h"

#include <cocos2d.h>

#include <iostream>
#include <sstream>
#include <map>
#include <string>
#include <vector>
#include <stdexcept>
#include <algorithm>


using namespace cocos2d;
using namespace std;

// TODO: \n in the middle text " some\ntext " check

string toLower(const string& str) {
    string res = str;
    transform(res.begin(), res.end(), res.begin(), ::tolower);
    return res;
}

LocalizationManager* LocalizationManager::getInstance() {
    static LocalizationManager instance;
    return &instance;
}

LocalizationManager::LocalizationManager() {
    auto curLanguage = SettingsManager::getInstance()->getLanguage(); 
    auto langs = getAvailableLanguages();
    if(find(langs.begin(), langs.end(), curLanguage) == langs.end())
	curLanguage = LanguageType::ENGLISH;
    loadLanguage(curLanguage);
}

void LocalizationManager::setLanguage(LanguageType lang) {
    SettingsManager::getInstance()->setLanguage(lang);
    loadLanguage(lang);
}

vector<LanguageType> LocalizationManager::getAvailableLanguages() {
    vector<LanguageType> langs;
    langs.push_back(LanguageType::ENGLISH);
    langs.push_back(LanguageType::RUSSIAN);
    return langs;
}

Sprite* LocalizationManager::getSpriteForLanguage(LanguageType lang) {
    auto filepath = "locales/" + toLower(getNameForLanguage(lang)) + ".png";
    auto sprite = Sprite::create(filepath);
//    sprite->setContentSize(Size(178, 105));
    return sprite;
}

std::string LocalizationManager::getNameForLanguage(LanguageType lang) {
    switch(lang) {
	case LanguageType::ENGLISH:
	    return "English";
	case LanguageType::RUSSIAN:
	    return "Russian";
	default:
	    return "English";
    }
}

string replace(const string& str) {
    string res;
    for(size_t i = 0; i < str.size(); i++) {
	if(i + 1 < str.size() && str.at(i) == '\\' && str.at(i+1) == 'n') {
	    i += 2;
	    res.push_back('\n');
	} else {
	    res.push_back(str.at(i));
	}
    }
    return res;
}

void LocalizationManager::loadLanguage(LanguageType lang) {
    string filePath = "locales/";
    string name = getNameForLanguage(lang);
    transform(name.begin(), name.end(), name.begin(), ::tolower);
    filePath += name + ".txt";

    _localizedStrings.clear();
    
    istringstream fileStringStream(
	FileUtils::getInstance()->getStringFromFile(
	    FileUtils::getInstance()->fullPathForFilename(filePath)
    ));
    
    string line;
    while (std::getline(fileStringStream, line)) {
	// starts with comment
	if(line.size() >= 2 && line.at(0) == '/' && line.at(1) == '/')
	    continue;
 	size_t delimPos = line.find(": ");
	if(delimPos == std::string::npos)
	    continue;
	string key = line.substr(0, delimPos);
	string value = replace(line.substr(delimPos + 2));
	_localizedStrings.insert(make_pair(key, value));
    }
    
}

string LocalizationManager::localize(const string& key) {
    auto it = _localizedStrings.find(key);
    if(it == _localizedStrings.end())
	return key;
    return it->second;
}
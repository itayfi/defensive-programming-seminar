//
//  SettingsStore.hpp
//  StatsClient
//
//  Created by Itay Fintz on 24/10/2020.
//  Copyright © 2020 Itay Fintz. All rights reserved.
//

#ifndef SettingsStore_hpp
#define SettingsStore_hpp

#include <string>

#include <nlohmann/json.hpp>

using json = nlohmann::json;

class SettingsStore {
private:
    static SettingsStore* instance;
    
    json data;
    
    SettingsStore();
public:
    static SettingsStore* getInstance();
    
    bool has(std::string key);
    std::string get(std::string key);
    void set(std::string key, std::string value);
};

#endif /* SettingsStore_hpp */

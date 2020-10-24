//
//  SettingsStore.cpp
//  StatsClient
//
//  Created by Itay Fintz on 24/10/2020.
//  Copyright © 2020 Itay Fintz. All rights reserved.
//

#include "SettingsStore.hpp"
#include <iostream>
#include <fstream>
#include <stdlib.h>

const json DEFAULT_SETTINGS = {
    {"server", "https://localhost:8002"}
};

SettingsStore* SettingsStore::instance = NULL;

SettingsStore* SettingsStore::getInstance() {
    if (instance == NULL) {
        instance = new SettingsStore;
    }
    
    return instance;
}

SettingsStore::SettingsStore() {
    std::string home = getenv("HOME");
    std::ifstream file(home + "/.stats-client.json");
    
    if (!file.is_open()) {
        std::cerr << "Could not read settings, using default" << std::endl;
        data = DEFAULT_SETTINGS;
    } else {
        file >> data;
        file.close();
    }
}

std::string SettingsStore::get(std::string key) {
    return data[key].get<std::string>();
}

void SettingsStore::set(std::string key, std::string value) {
    std::string home = getenv("HOME");
    std::ofstream file(home + "/.stats-client.json");
    
    data[key] = value;
    
    if (file.is_open()) {
        file << std::setw(4) << data;
        file.close();
    } else {
        std::cerr << "Could not save settings, continuing anyway" << std::endl;
    }
}



//
//  RecordCommand.cpp
//  StatsClient
//
//  Created by Itay Fintz on 24/10/2020.
//  Copyright © 2020 Itay Fintz. All rights reserved.
//

#include "RecordCommand.hpp"
#include "../HTTPClient.hpp"
#include "../UserException.hpp"
#include "../SettingsStore.hpp"

using json = nlohmann::json;

const std::string RecordCommand::getName() const {
    return "record";
}

const void RecordCommand::verify(const std::vector<std::string> args) const {
    if (args.size() != 2) {
        throw UserException("usage: stats-client record <tag> <value>", ERR_INVALID_ARGS);
    }
    try {
        std::stod(args[1]);
    } catch (std::invalid_argument& e) {
        throw UserException("Value should be a number", ERR_INVALID_ARGS);
    }
}

const void RecordCommand::run(const std::vector<std::string> args) const { 
    std::string tag = args[0];
    double value = std::stod(args[1]);
    
    json request = {
        {"tag", tag},
        {"value", value}
    };
    SettingsStore* settings = SettingsStore::getInstance();
    std::string accessToken = settings->get("access_token");
    HTTPClient::post(settings->get("server") + "/stats", request, &accessToken);
}



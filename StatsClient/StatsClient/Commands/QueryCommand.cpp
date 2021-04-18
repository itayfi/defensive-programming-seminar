//
//  QueryCommand.cpp
//  StatsClient
//
//  Created by Itay Fintz on 24/10/2020.
//  Copyright © 2020 Itay Fintz. All rights reserved.
//

#include <algorithm>
#include <map>
#include <vector>
#include <iostream>

#include "QueryCommand.hpp"
#include "../HTTPClient.hpp"
#include "../UserException.hpp"
#include "../SettingsStore.hpp"

using json = nlohmann::json;

const std::vector<std::string> VALID_AGGREGATIONS = {
    "count",
    "sum",
    "avg",
    "min",
    "max",
    "first",
    "last"
};
const std::vector<std::string> VALID_FILTERS = {"tag", "since", "until"};

const std::string QueryCommand::getName() const {
    return "query";
}

std::vector<std::string> getArgs(std::vector<std::string> args) {
    std::vector<std::string> results;
    
    std::copy_if(args.begin(), args.end(), std::back_inserter(results), [](std::string s){return s.find("--") != 0;});
    
    return results;
}

std::map<std::string, std::string> getNamedArgs(std::vector<std::string> args) {
    std::map<std::string, std::string> results;
    
    for (std::string arg : args) {
        if (arg.find("--") != 0) {
            continue;
        }
        size_t eqPos = arg.find("=");
        if (eqPos == std::string::npos) {
            throw UserException("Missing value for parameter " + arg, ERR_INVALID_ARGS);
        }
        
        results[arg.substr(2, eqPos - 2)] = arg.substr(eqPos + 1);
    }
    
    return results;
}

const void QueryCommand::verify(const std::vector<std::string> args) const {
    std::vector<std::string> aggs = getArgs(args);
    std::map<std::string, std::string> filters = getNamedArgs(args);
    
    for (std::string agg : aggs) {
        if (!std::count(VALID_AGGREGATIONS.begin(), VALID_AGGREGATIONS.end(), agg)) {
            throw UserException("Invalid aggregation " + agg, ERR_INVALID_ARGS);
        }
    }
    
    for (std::pair<std::string, std::string> filter : filters) {
        if (!std::count(VALID_FILTERS.begin(), VALID_FILTERS.end(), filter.first)) {
            throw UserException("Invalid filter " + filter.first, ERR_INVALID_ARGS);
        }
    }
}

const void QueryCommand::run(const std::vector<std::string> args) const {
    std::vector<std::string> aggs = getArgs(args);
    std::map<std::string, std::string> filters = getNamedArgs(args);

    json request = filters;
    request["aggregations"] = aggs;
    SettingsStore* settings = SettingsStore::getInstance();
    if (!settings->has("access_token")) {
        throw UserException("You must be logged in in order to run this command", ERR_USER_NOT_LOGGED_IN);
    }
    std::string accessToken = settings->get("access_token");
    json results = HTTPClient::post(settings->get("server") + "/query", request, &accessToken);
    
    for (json result : results["results"]) {
        std::cout << result["tag"].get<std::string>() << std::endl;
        
        for (auto item : result.items()) {
            if (item.key() != "tag") {
                std::cout << "\t" << item.key() << "\t" << item.value().get<std::string>() << std::endl;
            }
        }
        
        std::cout << std::endl;
    }
}

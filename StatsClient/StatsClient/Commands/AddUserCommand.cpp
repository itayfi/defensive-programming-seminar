//
//  AddUserCommand.cpp
//  StatsClient
//
//  Created by Itay Fintz on 31/10/2020.
//  Copyright © 2020 Itay Fintz. All rights reserved.
//

#include <unistd.h>

#include "AddUserCommand.hpp"
#include "../UserException.hpp"
#include "../HTTPClient.hpp"
#include "../SettingsStore.hpp"


const std::string AddUserCommand::getName() const { 
    return "add-user";
}

const void AddUserCommand::verify(const std::vector<std::string> args) const { 
    if (args.size() != 1) {
        throw UserException("usage: stats-client add-user <username>", ERR_INVALID_ARGS);
    }
}

const void AddUserCommand::run(const std::vector<std::string> args) const { 
    std::string username = args[0];
    std::string password = getpass("New user's password: ");
    
    json request = {
        {"username", username},
        {"password", password}
    };
    SettingsStore* settings = SettingsStore::getInstance();
    if (!settings->has("access_token")) {
        throw UserException("You must be logged in in order to run this command", ERR_USER_NOT_LOGGED_IN);
    }
    std::string accessToken = settings->get("access_token");
    HTTPClient::post(settings->get("server") + "/users", request, &accessToken);
}



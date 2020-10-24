//
//  LoginCommand.cpp
//  StatsClient
//
//  Created by Itay Fintz on 17/10/2020.
//  Copyright © 2020 Itay Fintz. All rights reserved.
//
#include <unistd.h>
#include <iostream>

#include <nlohmann/json.hpp>

#include "LoginCommand.hpp"
#include "../HTTPClient.hpp"
#include "../UserException.hpp"
#include "../SettingsStore.hpp"

using json = nlohmann::json;

const std::string LoginCommand::getName() const {
    return "login";
}

const void LoginCommand::verify(const std::vector<std::string> args) const {
    if (args.size() > 2) {
        throw UserException("usage: stats-client login [<user> [<password>] ]", ERR_INVALID_ARGS);
    }
}

const void LoginCommand::run(const std::vector<std::string> args) const {
    std::string user;
    std::string password;
    
    if (args.size() >= 1) {
        user = args[0];
    } else {
        std::cout << "Username: ";
        std::cin >> user;
    }
    
    if (args.size() == 2) {
        password = args[1];
    } else {
        password = getpass("Password: ");
    }
    
    json request = {
        {"user", user},
        {"password", password}
    };
    SettingsStore* settings = SettingsStore::getInstance();
    json response = HTTPClient::post(settings->get("server") + "/login", request);
    
    settings->set("access_token", response["access_token"].get<std::string>());
    std::cout << "Logged in until: " << response["access_token_expires"].get<std::string>() << std::endl;
}

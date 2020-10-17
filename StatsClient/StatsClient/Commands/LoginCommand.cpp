//
//  LoginCommand.cpp
//  StatsClient
//
//  Created by Itay Fintz on 17/10/2020.
//  Copyright © 2020 Itay Fintz. All rights reserved.
//
#include <unistd.h>
#include <iostream>

#include "LoginCommand.hpp"
#include "../UserException.hpp"

const std::string LoginCommand::getName() const {
    return "login";
}

const void LoginCommand::verify(const std::vector<std::string> args) const {
    if (args.size() > 2) {
        throw UserException("usage: stats-client login [<user> [<password>] ]", 2);
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
    
    std::cout << "Will send: " << user << ":" << password << std::endl;
}

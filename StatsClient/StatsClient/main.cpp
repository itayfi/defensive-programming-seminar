//
//  main.cpp
//  StatsClient
//
//  Created by Itay Fintz on 17/10/2020.
//  Copyright © 2020 Itay Fintz. All rights reserved.
//

#include <exception>
#include <iostream>
#include <string>
#include <vector>

#include "Commands/Command.hpp"
#include "Commands/LoginCommand.hpp"
#include "UserException.hpp"

const LoginCommand LOGIN_COMMAND;
const std::vector<const Command*> ALL_COMMANDS {
    &LOGIN_COMMAND
};

const Command* getCommand(std::string name) {
    for (const Command* cmd : ALL_COMMANDS) {
        if (cmd->getName() == name) {
            return cmd;
        }
    }
    throw UserException("Command not found: " + name, ERR_NO_COMMAND);
}

int main(int argc, const char * argv[]) {
    if (argc < 2) {
        std::cerr << "No command supplied" << std::endl;
        return ERR_NO_COMMAND;
    }
    std::vector<std::string> args(argv + 2, argv + argc);
    std::string cmdName(argv[1]);
    
    try {
        const Command* cmd = getCommand(cmdName);
        cmd->verify(args);
        cmd->run(args);
    } catch (UserException exp) {
        std::cerr << exp.what() << std::endl;
        return exp.getReturnValue();
    } catch (std::exception exp) {
        std::cerr << "An unexpected error occured" << std::endl;
        return ERR_UNKNOWN;
    }
    return 0;
}

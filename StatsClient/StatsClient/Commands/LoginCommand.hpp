//
//  LoginCommand.hpp
//  StatsClient
//
//  Created by Itay Fintz on 17/10/2020.
//  Copyright © 2020 Itay Fintz. All rights reserved.
//

#ifndef LoginCommand_hpp
#define LoginCommand_hpp

#include <stdio.h>
#include <string>
#include <vector>

#include "Command.hpp"

class LoginCommand : public Command {
public:
    virtual const std::string getName() const;
    virtual const void verify(const std::vector<std::string> args) const;
    virtual const void run(const std::vector<std::string> args) const;
};

#endif /* LoginCommand_hpp */

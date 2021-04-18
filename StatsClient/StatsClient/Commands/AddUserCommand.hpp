//
//  AddUserCommand.hpp
//  StatsClient
//
//  Created by Itay Fintz on 31/10/2020.
//  Copyright © 2020 Itay Fintz. All rights reserved.
//

#ifndef AddUserCommand_hpp
#define AddUserCommand_hpp

#include <stdio.h>
#include <string>
#include <vector>

#include "Command.hpp"

class AddUserCommand : public Command {
public:
    virtual const std::string getName() const;
    virtual const void verify(const std::vector<std::string> args) const;
    virtual const void run(const std::vector<std::string> args) const;
};

#endif /* AddUserCommand_hpp */

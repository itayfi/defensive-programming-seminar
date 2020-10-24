//
//  QueryCommand.hpp
//  StatsClient
//
//  Created by Itay Fintz on 24/10/2020.
//  Copyright © 2020 Itay Fintz. All rights reserved.
//

#ifndef QueryCommand_hpp
#define QueryCommand_hpp

#include <stdio.h>
#include <string>
#include <vector>

#include "Command.hpp"

class QueryCommand : public Command {
public:
    virtual const std::string getName() const;
    virtual const void verify(const std::vector<std::string> args) const;
    virtual const void run(const std::vector<std::string> args) const;
};

#endif /* QueryCommand_hpp */

//
//  Command.hpp
//  StatsClient
//
//  Created by Itay Fintz on 17/10/2020.
//  Copyright © 2020 Itay Fintz. All rights reserved.
//

#ifndef Command_hpp
#define Command_hpp

#include <stdio.h>
#include <string>
#include <vector>

class Command {
public:
    virtual const std::string getName() const = 0;
    virtual const void verify(std::vector<std::string> args) const = 0;
    virtual const void run(std::vector<std::string> args) const = 0;
};

#endif /* Command_hpp */

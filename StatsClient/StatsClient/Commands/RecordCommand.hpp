//
//  RecordCommand.hpp
//  StatsClient
//
//  Created by Itay Fintz on 24/10/2020.
//  Copyright © 2020 Itay Fintz. All rights reserved.
//

#ifndef RecordCommand_hpp
#define RecordCommand_hpp

#include <stdio.h>
#include <string>
#include <vector>

#include "Command.hpp"

class RecordCommand : public Command {
public:
    virtual const std::string getName() const;
    virtual const void verify(const std::vector<std::string> args) const;
    virtual const void run(const std::vector<std::string> args) const;
};

#endif /* RecordCommand_hpp */

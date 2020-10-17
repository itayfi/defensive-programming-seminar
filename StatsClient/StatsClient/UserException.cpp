//
//  UserException.cpp
//  StatsClient
//
//  Created by Itay Fintz on 17/10/2020.
//  Copyright © 2020 Itay Fintz. All rights reserved.
//

#include "UserException.hpp"

UserException::UserException(const std::string message, int returnValue) : std::exception(),
m_message(message),
m_returnValue(returnValue)
{}

const char* UserException::what() const throw () {
    return m_message.c_str();
}

const int UserException::getReturnValue() {
    return m_returnValue;
}

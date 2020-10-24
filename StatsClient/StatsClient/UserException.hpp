//
//  UserException.hpp
//  StatsClient
//
//  Created by Itay Fintz on 17/10/2020.
//  Copyright © 2020 Itay Fintz. All rights reserved.
//

#ifndef UserException_hpp
#define UserException_hpp

#include <stdio.h>
#include <exception>
#include <string>

#define ERR_NO_COMMAND 1
#define ERR_INVALID_ARGS 2
#define ERR_CONNECTION_ERROR 3
#define ERR_RESPONSE_ERROR 4
#define ERR_SERVER_ERROR 5
#define ERR_UNKNOWN 255

class UserException : public std::exception {
private:
    const std::string m_message;
    int m_returnValue;
    
public:
    UserException(const std::string message, int returnValue);
    
    const char* what() const throw ();
    const int getReturnValue();
};

#endif /* UserException_hpp */

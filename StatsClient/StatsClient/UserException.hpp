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

//
//  HTTPClient.hpp
//  StatsClient
//
//  Created by Itay Fintz on 23/10/2020.
//  Copyright © 2020 Itay Fintz. All rights reserved.
//

#ifndef HTTPClient_hpp
#define HTTPClient_hpp

#include <stdio.h>
#include <string>
#include <nlohmann/json.hpp>

using json = nlohmann::json;


class HTTPClient {
private:
    static json parseResponse(std::string response);
public:
    static json get(std::string url, std::string* authentication);
    static json post(std::string url, json body);
    static json post(std::string url, json body, std::string* authentication);
};

#endif /* HTTPClient_hpp */

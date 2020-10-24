//
//  HTTPClient.cpp
//  StatsClient
//
//  Created by Itay Fintz on 23/10/2020.
//  Copyright © 2020 Itay Fintz. All rights reserved.
//

#include <sstream>

#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>
#include <curlpp/Exception.hpp>

#include "HTTPClient.hpp"
#include "UserException.hpp"

json HTTPClient::parseResponse(std::string response) {
    json result;
    try {
        result = json::parse(response);
    } catch (json::parse_error &e) {
        throw UserException("Invalid response from server", ERR_RESPONSE_ERROR);
    }
    
    if (!result.is_object() || !result.contains("success")) {
        throw UserException("Invalid response from server", ERR_RESPONSE_ERROR);
    }
    if (!result["success"].get<bool>()) {
        std::string message = result["message"].get<std::string>();
        throw UserException(message, ERR_SERVER_ERROR);
    }
    
    return result;
}

json HTTPClient::get(std::string url, std::string* accessToken) {
    std::ostringstream os;
    try {
        curlpp::Easy request;
        
        request.setOpt(new curlpp::options::Url(url));
        request.setOpt(new curlpp::options::SslVerifyPeer(false));
        request.setOpt(new curlpp::options::SslVerifyHost(false));

        os << request;
    } catch (curlpp::LogicError &e) {
        std::cerr << e.what() << std::endl;
        throw UserException(std::string("Could not connect to server: ") + e.what(), ERR_CONNECTION_ERROR);
    } catch (curlpp::RuntimeError &e) {
        std::cerr << e.what() << std::endl;
        throw UserException(std::string("Could not connect to server: ") + e.what(), ERR_CONNECTION_ERROR);
    }
    
    return parseResponse(os.str());
}

json HTTPClient::post(std::string url, json body, std::string* accessToken) {
    std::ostringstream os;
    try {
        curlpp::Easy request;
        std::string postData = body.dump();
        
        request.setOpt(new curlpp::options::Url(url));
        request.setOpt(new curlpp::options::SslVerifyPeer(false));
        request.setOpt(new curlpp::options::SslVerifyHost(false));
        
        std::list<std::string> header;
        header.push_back("Content-Type: application/json");
        if (accessToken != NULL) {
            header.push_back("Authorization: Bearer " + *accessToken);
        }
        
        request.setOpt(new curlpp::options::HttpHeader(header));
        
        request.setOpt(new curlpp::options::PostFields(postData));
        request.setOpt(new curlpp::options::PostFieldSize(postData.size()));
        
        os << request;
    } catch (curlpp::LogicError &e) {
        throw UserException(std::string("Could not connect to server: ") + e.what(), ERR_CONNECTION_ERROR);
    } catch (curlpp::RuntimeError &e) {
        throw UserException(std::string("Could not connect to server: ") + e.what(), ERR_CONNECTION_ERROR);
    }
    
    return parseResponse(os.str());
}

json HTTPClient::post(std::string url, json body) {
    return post(url, body, NULL);
}

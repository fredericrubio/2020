/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   InputParser.cpp
 * Author: fredericrubio
 * 
 * Created on 22 août 2019, 19:05
 */

#include "InputParser.hpp"

#include <algorithm>

InputParser::InputParser() {
}

InputParser::InputParser(const InputParser& orig) {
}

InputParser::~InputParser() {
}

InputParser::InputParser (int &argc, char **argv){
            for (int i=1; i < argc; ++i)
                this->tokens.push_back(std::string(argv[i]));
}
        
const std::string& InputParser::getCmdOption(const std::string &option) const{
    std::vector<std::string>::const_iterator itr;
    itr =  std::find(this->tokens.begin(), this->tokens.end(), option);
    if (itr != this->tokens.end() && ++itr != this->tokens.end()){
        return *itr;
    }
    static const std::string empty_string("");
    return empty_string;
}

/// @author iain
bool InputParser::cmdOptionExists(const std::string &option) const{
    return std::find(this->tokens.begin(), this->tokens.end(), option)
           != this->tokens.end();
}

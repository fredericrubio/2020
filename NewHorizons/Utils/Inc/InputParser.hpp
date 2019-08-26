/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   InputParser.hpp
 * Author: fredericrubio
 *
 * Created on 22 août 2019, 19:05
 */

#ifndef INPUTPARSER_HPP
#define INPUTPARSER_HPP

#include <vector>
#include <string>

class InputParser {
    
    public:

        InputParser();
        InputParser(const InputParser& orig);
        InputParser (int &argc, char **argv);
        virtual ~InputParser();
        const std::string& getCmdOption(const std::string &option) const;
        bool cmdOptionExists(const std::string &option) const;

    private:
        std::vector <std::string> tokens;
};

#endif /* INPUTPARSER_HPP */


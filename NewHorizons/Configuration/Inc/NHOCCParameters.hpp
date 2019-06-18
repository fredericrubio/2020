//
//  NHOServerParameters.hpp
//  Configuration
//
//  Created by Frédéric Rubio on 18/06/2019.
//  Copyright © 2019 Frédéric Rubio. All rights reserved.
//

#ifndef NHOServerParameters_hpp
#define NHOServerParameters_hpp

#include <string>

class NHOCCParameters {
    
public:
    
    /**
     * Constructor
     **/
    NHOCCParameters();
    
    /**
     * HostName
     **/
    inline void setHostName(const std::string& pHostname) {name = pHostname;};
    inline std::string getHostName() const {return name;};
    
    /**
     * Port
     **/
    inline void setPort(const unsigned short pPort) {port = pPort;}
    inline unsigned short getPort() const {return port;};

protected :
    std::string name;
    unsigned short port;

};
#endif /* NHOServerParameters_hpp */

//
//  NHOEmitter.cpp
//  Network
//
//  Created by Frédéric Rubio on 10/04/2019.
//  Copyright © 2019 Frédéric Rubio. All rights reserved.
//

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <errno.h>
#include <netdb.h>
#include <arpa/inet.h>

#include "NHOLOG.hpp"
#include "NHOMessage.hpp"
#include "NHOMessageFactory.hpp"
#include "NHOMessage.hpp"

#include "NHOEmitter.hpp"
/**
 * Member functions
 **/
NHOEmitter::NHOEmitter(const unsigned short pPort, const unsigned short pPeriod) {
    
    emissionPort = pPort;
    sampling = pPeriod;
    counter = 0;
    emissionSocket = 0;

}




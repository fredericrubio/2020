//
//  main.cpp
//  Hearing
//
//  Created by Frédéric Rubio on 05/09/2025.
//  Copyright © 2025 Frédéric Rubio. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <iostream>

#include "NHOLOG.hpp"
#include "NHOSolenoidValveMessage.hpp"
#include "NHOTemplateBroadcastReceiver.hpp"
#include "NHOSolenoidValveMessageDispatcher.hpp"
#include "NHOSolenoidValveMessageDispatcher.hpp"
#include "NHOSVPingPong.hpp"
#include "NHOTemplateBroadcaster.hpp"

int main(int argc, const char * argv[]) {
    
    NHOTemplateBroadcaster<NHOSolenoidValveMessage>* msgBroadcast = new NHOTemplateBroadcaster<NHOSolenoidValveMessage>(51718);
//    NHOSVEmRec* msgReceiver = new NHOSVEmRec(51718);
    bool lReturnH = msgBroadcast->initiate() ;
    if (lReturnH == false) {
        msgBroadcast->terminate();
        NHOFILE_LOG(logERROR)  << "main : hearing initialization failed." << std::endl;
        return -1;
    }
    
    msgBroadcast->attach((NHOSolenoidValveMessageDispatcher *) NHOSolenoidValveMessageDispatcher::get());
    
    NHOSVPingPong* pingPong = new NHOSVPingPong(msgBroadcast, "OSX Pong");
    NHOSolenoidValveMessageDispatcher::get()->subscribe(NHOSolenoidValveData::ePing, pingPong);

    while (true) {
        
        // sleep 1 second
        std::this_thread::sleep_for(std::chrono::milliseconds((long long) (1.0 * 1000)));
//        NHOFILE_LOG(logDEBUG)  << "main : hearing loop." << std::endl;

    }
    
    return 0;
}

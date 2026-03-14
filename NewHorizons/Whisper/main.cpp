//
//  main.cpp
//  Wishper
//
//  Created by Frédéric Rubio on 04/09/2025.
//  Copyright © 2025 Frédéric Rubio. All rights reserved.
//

#include <iostream>

#include <iostream>
#include <chrono>
#include <thread>

#include "NHOLOG.hpp"
#include "NHOSolenoidValveMessage.hpp"
#include "NHOBroadcastEmitter.hpp"
#include "NHOSolenoidValveMessageDispatcher.hpp"
#include "NHOTemplateBroadcaster.hpp"
#include "NHOSVCommandSender.hpp"
#include "NHOSVPingPong.hpp"

// 51717 / 51719
int main(int argc, const char * argv[]) {
    
    // initialize emitter
    bool keepRefreshing = true;

    NHOTemplateBroadcaster<NHOSolenoidValveMessage>* msgBroadcast = new NHOTemplateBroadcaster<NHOSolenoidValveMessage>(51718);
    bool lReturnH = msgBroadcast->initiate() ;
    if (lReturnH == false) {
        msgBroadcast->terminate();
        NHOFILE_LOG(logERROR)  << "main : hearing initialization failed." << std::endl;
        exit(1) ;
    }
    msgBroadcast->attach((NHOSolenoidValveMessageDispatcher *) NHOSolenoidValveMessageDispatcher::get());

    NHOSVCommandSender* commandSender = new NHOSVCommandSender(msgBroadcast, "Solenoid Valve Manager");
    NHOSolenoidValveMessageDispatcher::get()->subscribe(NHOSolenoidValveData::eAckMessage, commandSender);
    NHOSolenoidValveMessageDispatcher::get()->subscribe(NHOSolenoidValveData::eNackMessage, commandSender);

    NHOSVPingPong* pingPong = new NHOSVPingPong(msgBroadcast, "OSX Pong");
    NHOSolenoidValveMessageDispatcher::get()->subscribe(NHOSolenoidValveData::ePing, pingPong);
    pingPong->setDelayPing(1000);
    pingPong->setDelayAck(500);
    
    // = new NHOSolenoidValveMessage(clock());
    bool result = false;
    
    while (keepRefreshing) {
        
        // sleep 1 second
        std::this_thread::sleep_for(std::chrono::milliseconds((long long) (1 * 1000)));
        // send mesg and get status
        result = commandSender->send(NHOSolenoidValveData::eClose);
        if (! result) {
            NHOFILE_LOG(logERROR) << "Main : error on send message." << std::endl;
        }
        
        // sleep 2 second
        std::this_thread::sleep_for(std::chrono::milliseconds((long long) (2 * 1000)));
        // send mesg and get status
        result = commandSender->send(NHOSolenoidValveData::eOpen);
        if (! result) {
            NHOFILE_LOG(logERROR) << "Main : error on send message msg2." << std::endl;
        }
        
        commandSender->loop();
    }
    return 0;
}

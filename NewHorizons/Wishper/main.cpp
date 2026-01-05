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

// 51717 / 51719
int main(int argc, const char * argv[]) {
    
    // initialize emitter
    NHOBroadcastEmitter* dataEmitter = new NHOBroadcastEmitter(51718, 1000);
    if (!dataEmitter->initiate()) {
            exit(-1);
    }
    bool keepRefreshing = true;
    const NHOSolenoidValveData* data = new NHOSolenoidValveData(NHOSolenoidValveData::ePong, clock());
    NHOSolenoidValveMessage* msg = dynamic_cast<NHOSolenoidValveMessage*>(NHOMessageFactory::build(data));
    msg->computeSize();
    msg->serialize();
    // = new NHOSolenoidValveMessage(clock());
    bool result = false;
    
    while (keepRefreshing) {
        
        // sleep 1 second
        std::this_thread::sleep_for(std::chrono::milliseconds((long long) (1 * 1000)));
        
        // send mesg and get status
        result = dataEmitter->send(msg);
        if (! result) {
            NHOFILE_LOG(logERROR) << "Main : error on send message." << std::endl;
        }
    }
    return 0;
}

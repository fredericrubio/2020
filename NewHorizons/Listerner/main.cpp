//
//  main.cpp
//  Listerner
//
//  Created by Frédéric Rubio on 07/08/2022.
//  Copyright © 2022 Frédéric Rubio. All rights reserved.
//

#include <iostream>
#include <chrono>
#include <thread>

#include "NHOProxyAlertCenter.hpp"
#include "NHOHallEffectData.hpp"

#include "NHOSoleniodValveMessage.hpp"

int main(int argc, const char * argv[]) {
    
    // initialize emitter
    NHOProxyAlertCenter::initialize();
    bool keepRefreshing = true;
    const NHOSoleniodValveMessage* msg = new NHOSoleniodValveMessage();
    
    while (keepRefreshing) {
        
        // sleep 1 second
        std::this_thread::sleep_for(std::chrono::milliseconds((long long) (0.5 * 1000)));

#ifdef HALL_EFFECT_TEST
        // get values
        const NHOHallEffectMessage* msg = NHOProxyAlertCenter::getMessage();
        if (msg == NULL) {
            exit(1);
        }
        
        if (msg->getHEData())
            std::cout << "Main - " << msg->getDate() << " Hall Effect: " << msg->getHEData()->getHallEffect() << "\n";
#else
        // send ping
        
#endif
        
    }
    
    // insert code here...
    std::cout << "Hello, World!\n";
    return 0;
}

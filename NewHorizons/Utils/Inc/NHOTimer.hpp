//
//  NHOTimer.hpp
//  Utils
//
//  Created by Frédéric Rubio on 10/06/2019.
//  Copyright © 2019 Frédéric Rubio. All rights reserved.
//

#ifndef NHOTimer_hpp
#define NHOTimer_hpp

#include <mutex>
#include <thread>
#include <unistd.h>

#include "NHOSubject.hpp"

class NHOTimer: public NHOSubject<unsigned int> {
    
public:

    NHOTimer() {
        this->value = new unsigned int();
        ticking = false;
        this->observers.clear();
    }

    void start(const unsigned int pValue){
        if (isTicking()) {
            stop();
        }
        
        elapsedTime = 0;
        target = pValue * 1000;
        thread = new std::thread(&NHOTimer::timer, std::ref(*this));
    }

    void stop() {
        if (!ticking) {
            return;
        }
        
        mutex.lock();
        ticking = false;
        mutex.unlock();
        while (isTicking()) {
            // wait for effective stop
            usleep(10);
        }
    }

    inline unsigned int getElapsedTime() const {return (unsigned int) ((float) elapsedTime) * 0.001;};

    inline bool isTicking() const {return ticking;};

protected:

    unsigned int elapsedTime; // microseconds
    unsigned int target;
    bool ticking;
    std::mutex mutex;
    std::thread* thread;

    /**
     * 
     **/
    void timer() {
        unsigned int timeStep = 100; // microseconds
        elapsedTime = 0;
        ticking = true;
        while (isTicking()) {
            usleep(timeStep);
            elapsedTime += timeStep;
            if (elapsedTime >= target) {
                mutex.lock();
                ticking = false;
                // set and ocnvert into millisecons
                *value = (unsigned int) (((float) elapsedTime) * 0.001);
                notify();
                mutex.unlock();
            }
        }
    }
    
};
#endif /* NHOTimer_hpp */

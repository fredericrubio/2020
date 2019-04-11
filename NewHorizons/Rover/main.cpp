//
//  main.cpp
//  Rover
//
//  Created by Frédéric Rubio on 30/03/2019.
//  Copyright © 2019 Frédéric Rubio. All rights reserved.
//

#include "NHOLOG.hpp"

#include "NHOCamera.hpp"

int main(int argc, const char * argv[]) {
    
    NHOCamera*  lCamera = new NHOCamera();
    
    // insert code here...
    NHOFILE_LOG(logDEBUG) << "main: start" << std::endl;
    
    return 0;
    
}

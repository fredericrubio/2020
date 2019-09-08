//
//  NHOWiringPi.hpp
//  Utils
//
//  Created by Frédéric Rubio on 01/06/2019.
//  Copyright © 2019 Frédéric Rubio. All rights reserved.
//

#ifndef NHOWiringPi_hpp
#define NHOWiringPi_hpp

#include <stdio.h>

class NHOWiringPi {
    
public:
    
    static int ERROR_CODE;
    
    static const unsigned int MOTOR1_MISSING = 0;
    static const unsigned int MOTOR1_NOT_RESPONDING = 1;
    static const unsigned int MOTOR2_NOT_RESPONDING = 2;
    static const unsigned int MOTOR2_MISSING = 3;

    static const unsigned int INCORRECT_TC = 10;
    static const unsigned int UNSTOPPABLE_TC = 11;
    static const unsigned int UNKNOWN_TC = 12;

    static const int TOTAL_GPIO_PINS = 40;

    static void PrintModes();
    static void PrintDigitalValues();
    static void PrintModes(const int pModes[]);
    static void PrintDigitalValues(const unsigned short pDigitalValues[]);

#ifndef _RASPBIAN

    // Maximum number of GPIO pins available (other 12 are dedicated to power supply)
    static const int GPIO_PINS = 28;
    // there are gaps between the number of avaiblable gpio pins. It is not a continuous sequence.
    // numbers from 17 to 20 are not used to address a pin.
    static const unsigned short WiringPiMap[];

    // The following constants are a copy from wiringPi.h.
    // Necessary to undestand the other side
    
    // C doesn't have true/false by default and I can never remember which
    //    way round they are, so ...
    //    (and yes, I know about stdbool.h but I like capitals for these and I'm old)
    #ifndef    TRUE
    #  define    TRUE    (1==1)
    #  define    FALSE    (!TRUE)
    #endif
    
    // GCC warning suppressor
    #define    UNU    __attribute__((unused))
    
    // Mask for the bottom 64 pins which belong to the Raspberry Pi
    //    The others are available for the other devices
    #define    PI_GPIO_MASK    (0xFFFFFFC0)
    
    // Handy defines

    // wiringPi modes
    #define    WPI_MODE_PINS         0
    #define    WPI_MODE_GPIO         1
    #define    WPI_MODE_GPIO_SYS     2
    #define    WPI_MODE_PHYS         3
    #define    WPI_MODE_PIFACE         4
    #define    WPI_MODE_UNINITIALISED    -1
    
    // Pin modes
    static const int    INPUT = 0;
    static const int    OUTPUT = 1;
    #define    PWM_OUTPUT         2
    #define    GPIO_CLOCK         3
    #define    SOFT_PWM_OUTPUT         4
    #define    SOFT_TONE_OUTPUT     5
    #define    PWM_TONE_OUTPUT         6
    
    static const int    LOW = 0;
    static const int    HIGH = 1;
    
    // Pull up/down/none
    #define    PUD_OFF             0
    #define    PUD_DOWN         1
    #define    PUD_UP             2
    
    // PWM
    #define    PWM_MODE_MS        0
    #define    PWM_MODE_BAL        1
    
    // Interrupt levels
    #define    INT_EDGE_SETUP        0
    #define    INT_EDGE_FALLING    1
    #define    INT_EDGE_RISING        2
    #define    INT_EDGE_BOTH        3

#endif
    
    struct WiringPiNodeStruct
    {
        int     pinBase ;
        int     pinMax ;
        
        int          fd ;    // Node specific
        unsigned int data0 ;    //  ditto
        unsigned int data1 ;    //  ditto
        unsigned int data2 ;    //  ditto
        unsigned int data3 ;    //  ditto
        
        struct WiringPiNodeStruct *next ;
    } ;

    static struct WiringPiNodeStruct *WiringPiFindNode (int pin) ;
    static struct WiringPiNodeStruct *WiringPiNewNode  (int pinBase, int numPins) ;
    
    // SETUP section
    static void WiringPiVersion    (int *major, int *minor) ;
    static  int WiringPiSetup     (void) ;
    static int  WiringPiSetupSys   (void) ;
    static int  WiringPiSetupGpio  (void) ;
    static int  WiringPiSetupPhys  (void) ;
    
    // the hardware PWM pins are BCM_18 (pwm0) and BCM_13 (pwm1).
    // Or wiringPi pins 1 and 23 respectively, or physical pins 12 and 33.
    // It's also possible to do:
    // pinMode (pin, SOFT_PWM_OUTPUT) ;
    // which does the same as softPwmCreate() ; You still need to use
    // softPwmWrite (pin, value) ; though.
    static int  SoftPwmCreate (int pin, int value, int range) ;
    static void SoftPwmWrite  (int pin, int value) ;
    static void SoftPwmStop   (int pin) ;
    
    static void PinModeAlt          (int pin, int mode) ;
    static void PinMode             (int pin, int mode) ; // GPIO.setup
    static void PullUpDnControl     (int pin, int pud) ;
    static int  DigitalRead         (int pin) ;
    static void DigitalWrite        (int pin, int value) ;
    static unsigned int  DigitalRead8        (int pin) ;
    static void DigitalWrite8       (int pin, int value) ;
    static void PwmWrite            (int pin, int value) ; // GPIO.PWM
    static int  AnalogRead          (int pin) ;
    static void AnalogWrite         (int pin, int value) ;
    static int  GetAlt              (int pin) ;
    
};
#endif /* NHOWiringPi_hpp */

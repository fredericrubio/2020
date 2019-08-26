//
//  ViewController.h
//  CommandCenter
//
//  Created by Frédéric Rubio on 01/06/2019.
//  Copyright © 2019 Frédéric Rubio. All rights reserved.
//

#import <Cocoa/Cocoa.h>

#include <thread>

@interface ViewController : NSViewController

@property (strong) IBOutlet NSImageView *cameraView;
@property (strong) IBOutlet NSImageView *testImage;

@property (strong) IBOutlet NSButton *forwardButton;
@property (strong) IBOutlet NSButton *reverseButton;
@property (strong) IBOutlet NSButton *stopButton;
@property (strong) IBOutlet NSButton *turnLeftButton;
@property (strong) IBOutlet NSButton *turnRightButton;
@property (strong) IBOutlet NSButton *network;
@property (strong) IBOutlet NSTextField *cpuTextField;
@property (strong) IBOutlet NSTextField *memoryTextFiled;
@property (strong) IBOutlet NSTextField *temperatureTextField;
@property (strong) IBOutlet NSButton *gpioMode1;
@property (strong) IBOutlet NSMatrix *gpioModeMatrix;
@property (strong) IBOutlet NSMatrix *pwmMatrix;
@property (strong) IBOutlet NSMatrix *gpioDigitalValueMatrix;

@property bool networkStatus;
@property bool keepRefreshing;
@property NSString *cameraCapture;
@property (strong) IBOutlet NSButton *cameraLinkStatus;

- (IBAction)forwardCallback:(id)sender;
- (IBAction)reverseCallback:(id)sender;

- (IBAction)stopCallback:(id)sender;
- (IBAction)turnRightCallback:(id)sender;
- (IBAction)turnLeftCallback:(id)sender;

- (void)refreshCameraView;
- (void)refreshHEM;

@end


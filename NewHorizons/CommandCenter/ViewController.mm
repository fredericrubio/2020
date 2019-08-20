//
//  ViewController.m
//  CommandCenter
//
//  Created by Frédéric Rubio on 01/06/2019.
//  Copyright © 2019 Frédéric Rubio. All rights reserved.
//

#import "ViewController.h"

#include "NHOCommandCenter.hpp"
#include "NHOHEMData.hpp"

@implementation ViewController

- (void) viewDidLoad {
    [super viewDidLoad];
    
    self.keepRefreshing = true;
    
    NSString *fileName = @"/Users/fredericrubio/Desktop/IMG_0113.png";
    NSImage *image = [[NSImage alloc] initWithContentsOfFile:fileName];
    image = [NSImage imageNamed:@"Encelade.png"];
//    if (image == nil) {
//        NSLog(@"image nil");
//    }
//    else {
//        NSLog(@"%@", image.name);
//        NSLog(@"size %f %f",image.size.width,image.size.height);
//    }
    [self.cameraView setImage:image];
    self.cameraCapture = @"";

    // initialize emitter
    self.networkStatus = NHOCommandCenter::initialize();

    // CAMERA VIEW
    /// refreshing the camera view
    NSThread* cameraThread = [ [NSThread alloc] initWithTarget:self
                                                   selector:@selector( refreshCameraView )
                                                     object:nil ];
    /// start thread
    [cameraThread start];
    
    // HEM
    /// refreshing HEM
    NSThread* hemThread = [ [NSThread alloc] initWithTarget:self
                                                   selector:@selector( refreshHEM )
                                                     object:nil ];
    /// start thread
    [hemThread start];
    
    // manage Quit
    NSNotificationCenter* nc = [NSNotificationCenter defaultCenter];
    [nc addObserver:self
           selector:@selector(closeConnexion)
               name:NSApplicationWillTerminateNotification
             object:nil];
    
    // initialize Mode Matrix
    int row = 0;
    int column = 0;
    NSButtonCell *cell;
    for (row = 0 ; row < [self.gpioModeMatrix numberOfRows] ; row++) {
        for (column = 0 ; column < [self.gpioModeMatrix numberOfColumns] ; column++) {
              cell = [self.gpioModeMatrix cellAtRow:row column:column];
              [cell setImage:[NSImage imageNamed:@"Red.png"]];
        }
    }
}
/**
 * Close connexion, end loops
 **/
- (void) closeConnexion {
    
    self.keepRefreshing = false;
    
    NHOCommandCenter::terminate();

}

/**
 * Get the latest 'ppm' image in the directory containing
 * the camera captures.
 **/
- (NSString*) getLatestsFile {
    
    NSString* rootPath = @"/Users/fredericrubio/Development/Project/New Horizons/Development/NewHorizons/DerivedData/Build/Products";
    
    NSFileManager *fileManager = [NSFileManager defaultManager];
    NSURL *directoryURL = [NSURL fileURLWithPath:rootPath];
    NSArray *keys = [NSArray arrayWithObject:NSURLIsDirectoryKey];
    
    NSDirectoryEnumerator *enumerator = [fileManager
                                         enumeratorAtURL:directoryURL
                                         includingPropertiesForKeys:keys
                                         options:0
                                         errorHandler:^(NSURL *url, NSError *error) {
                                             // Handle the error.
                                             // Return YES if the enumeration should continue after the error.
                                             return YES;
                                         }];
    
    NSDate *lastModifiedDate = [NSDate dateWithTimeIntervalSince1970:0];
    NSString *lastModifiedFilePath = @"";
    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSLibraryDirectory, NSUserDomainMask, YES);
    NSString *fontPath = [[paths objectAtIndex:0] stringByAppendingPathComponent:@"Fonts"];
    BOOL isDir;

    // loop on directory contents
    for (NSURL *url in enumerator) {
        [fileManager fileExistsAtPath:[url path] isDirectory:&isDir];
        NSRange isRange = [[url absoluteString] rangeOfString:@".ppm" options:NSCaseInsensitiveSearch];
        
        if ((! isDir)
            &&
            (isRange.length != 0)) {
            NSDate* creationDate;
            NSError* error;
            if ([url getResourceValue:&creationDate forKey:NSURLCreationDateKey error:&error]) {
                if (lastModifiedDate > creationDate) {
                    lastModifiedDate = creationDate;
                    lastModifiedFilePath = [url path];
                }
            }
            else {
                NSLog(@"No creation date");
            }
        }
    }
    
//    NSLog(@"Lastest modified file: <%@>", lastModifiedFilePath);
    return lastModifiedFilePath;
}

- (NSString *)valueForKey:(NSString *)key
           fromQueryItems:(NSArray *)queryItems
{
    NSPredicate *predicate = [NSPredicate predicateWithFormat:@"name=%@", key];
    NSURLQueryItem *queryItem = [[queryItems
                                  filteredArrayUsingPredicate:predicate]
                                 firstObject];
    return queryItem.value;
}

- (void)refreshCameraView {
    
    // refresh network status
    if (self.networkStatus == false) {
        [self.network setImage:[NSImage imageNamed:@"Red.png"]];
    }
    
    NSString *imageToDisplayAbsPath = @"";
    
    while (self.keepRefreshing) {
        
        [NSThread sleepForTimeInterval:1];
        imageToDisplayAbsPath = [self getLatestsFile];
        if (imageToDisplayAbsPath != nil) {
//            if (! [imageToDisplayAbsPath isEqualToString:self.cameraCapture]) {
            if (! [imageToDisplayAbsPath isEqualToString:[NSString stringWithUTF8String:""]]) {
                [self performSelectorOnMainThread:@selector(setNewImage:) withObject:imageToDisplayAbsPath waitUntilDone:YES];
            }
        }
    }
    
    NSLog(@"End of refreshCemaeraView");

}

/**
 *
 **/
- (void)refreshHEM {
    
    if (self.networkStatus == true) {
        
        while (self.keepRefreshing) {

            // sleep 1 second
            [NSThread sleepForTimeInterval:0.5];
            
            [self performSelectorOnMainThread:@selector(setHEM) withObject:nil waitUntilDone:YES];
        }
        
    }

    NSLog(@"End of refreshCemaeraView");

}

/**
 * Refresh HEM GUI section
 **/
-(void) setHEM {
    
    // get values
    const NHOHEMMessage* hem = NHOCommandCenter::getHEM();
    
    if (hem == NULL) {
        return;
    }
    // CPU
    [_cpuTextField setStringValue:[NSString stringWithFormat:@"%d", hem->getHEMData()->getCPUUsage()]];

    // MEMORY
    [_memoryTextFiled setStringValue:[NSString stringWithFormat:@"%d", hem->getHEMData()->getMemoryUsage()]];
    
    // TEMPERATURE
    [_temperatureTextField setStringValue:[NSString stringWithFormat:@"%d", hem->getHEMData()->getTemperature()]];

    // MODES
    int counter = 0;
    int row = 0;
    int column = 0;
    NSButtonCell *cell;
    for (row = 0 ; row < [self.gpioModeMatrix numberOfRows] ; row++) {
        for (column = 0 ; column < [self.gpioModeMatrix numberOfColumns] ; column++) {
            cell = [self.gpioModeMatrix cellAtRow:row column:column];
            if (hem->getHEMData()->getPinModes()[counter] == NHOWiringPi::OUTPUT) {
                [cell setImage:[NSImage imageNamed:@"Green.png"]];
            }
            else {
                [cell setImage:[NSImage imageNamed:@"Red.png"]];
            }
            counter++;
        }
    }

    // DIGITAL VALUES
    counter = 0;
    row = 0;
    column = 0;
    for (row = 0 ; row < [self.gpioDigitalValueMatrix numberOfRows] ; row++) {
        for (column = 0 ; column < [self.gpioDigitalValueMatrix numberOfColumns] ; column++) {
            cell = [self.gpioDigitalValueMatrix cellAtRow:row column:column];
            if (hem->getHEMData()->getDigitalValues()[counter] == NHOWiringPi::HIGH) {
                [cell setImage:[NSImage imageNamed:@"Green.png"]];
            }
            else {
                [cell setImage:[NSImage imageNamed:@"Red.png"]];
            }
            counter++;
        }
    }

    delete hem;
    
}

-(void) setNewImage:(NSString*)imageToLoadAbsPath {
    
    if (imageToLoadAbsPath == nil) {
        NSLog(@"Image to display is 'nil'");
        [self.cameraLinkStatus setImage:[NSImage imageNamed:@"Red.png"]];
        return;
    }
    
    if ([imageToLoadAbsPath isEqualToString:self.cameraCapture]) {
        [self.cameraLinkStatus setImage:[NSImage imageNamed:@"Orange.png"]];
    }
    else {
        // store the displayed image
        self.cameraCapture = imageToLoadAbsPath;
        [self.cameraLinkStatus setImage:[NSImage imageNamed:@"Green.png"]];
        
        NSImage *image = [[NSImage alloc] initWithContentsOfFile:self.cameraCapture];
        [self.cameraView setImage:image];
        
        // memory management
        image = nil;
    }

}

- (void)setRepresentedObject:(id)representedObject {
    [super setRepresentedObject:representedObject];

    // Update the view, if already loaded.
    
}


- (IBAction)forwardCallback:(id)sender {
    
    NHOCommandCenter::sendForwardTC();
    
}

- (IBAction)reverseCallback:(id)sender {
    
    NHOCommandCenter::sendReverseTC();
    
}

- (IBAction)stopCallback:(id)sender {
    
    NHOCommandCenter::sendStopTC();
    
}

- (IBAction)turnRightCallback:(id)sender {
    
    NHOCommandCenter::sendTurnRightTC();
    
}

- (IBAction)turnLeftCallback:(id)sender {
    
    NHOCommandCenter::sendTurnLeftTC();
    
}
@end

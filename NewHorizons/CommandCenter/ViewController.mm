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

//    if (image == nil) {
//        NSLog(@"image nil");
//    }
//    else {
//        NSLog(@"%@", image.name);
//        NSLog(@"size %f %f",image.size.width,image.size.height);
//    }
    [self.cameraView setImage:image];

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

- (NSString*) getLatestsFile {
    
    NSString* rootPath = @"/Users/fredericrubio/Development/Project/New Horizons/Development/NewHorizons/DerivedData/Build/Products";
    
//    BOOL isDir = TRUE;
//    [[NSFileManager defaultManager] fileExistsAtPath:path
//                                         isDirectory:&isDir];
//    if ( isDir ) {
//        NSMutableArray *contentItemArray = [[NSMutableArray alloc] init];
//        NSArray *contentOfDirectory =
//        [[NSFileManager defaultManager] contentsOfDirectoryAtPath:path
//                                                            error:NULL];
//
//        for (int i = 0; i<[contentOfDirectory count]; i++) {
//
//            NSString *fileName = [contentOfDirectory objectAtIndex:i];
//
//            if([fileName.pathExtension isEqualToString:@"png"])
//            {
//                [contentItemArray addObject:fileName];
//                NSURL *fileUrl = [NSURL URLWithString:path];
//                NSDate *fileDate;
//                [fileName getResourceValue:&fileDate forKey:NSURLContentModificationDateKey error:&error];
//                if (!error)
//                {
//                    //here you should be able to read valid date from fileDate variable
//                }
//            }
//        }
//
//    }
    
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
    
    for (NSURL *url in enumerator) {
        
//        NSError *error;
//        NSNumber *isDirectory = nil;
//        NSString *pathToTest = nil;
        
//        NSMutableDictionary *queryStrings = [[NSMutableDictionary alloc] init];
//        for (NSString *qs in [url.query componentsSeparatedByString:@"&"]) {
//            // Get the parameter name
//            NSString *key = [[qs componentsSeparatedByString:@"="] objectAtIndex:0];
//            // Get the parameter value
//            NSString *value = [[qs componentsSeparatedByString:@"="] objectAtIndex:1];
//            value = [value stringByReplacingOccurrencesOfString:@"+" withString:@" "];
//            value = [value stringByReplacingPercentEscapesUsingEncoding:NSUTF8StringEncoding];
//
//            queryStrings[key] = value;
//        }
        
//        NSLog(@"<%@> ", [url path]);
        [fileManager fileExistsAtPath:[url path] isDirectory:&isDir];
        NSRange isRange = [[url absoluteString] rangeOfString:@".ppm" options:NSCaseInsensitiveSearch];
//        if(isRange.length != 0) {
//            NSLog(@"==> image");
//        }
        
        if ((! isDir)
            &&
            (isRange.length != 0)) {
            NSDate* creationDate;
            NSError* error;
            if ([url getResourceValue:&creationDate forKey:NSURLCreationDateKey error:&error]) {
                if (lastModifiedDate > creationDate) {
                    lastModifiedDate = creationDate;
                    lastModifiedFilePath = [url absoluteString];
                }
            }
            else {
                NSLog(@"No creation date");
            }
        }
//        if (url.hasDirectoryPath) {
//            NSLog(@"<%@> is a directory.", url);
//        }
//
//        if (! [url getResourceValue:&isDirectory forKey:NSURLIsDirectoryKey error:&error]) {
//            // handle error
//        }
//        else {
//            NSLog(@"<%@> is not a directory.", url);
//            Boolean directory = [isDirectory boolValue];
//            if (! [isDirectory boolValue]) {
//                // No error and it’s not a directory; do something with the file
//                //            NSLog(@"%@", [url absoluteString]);
//                NSDate* creationDate;
//                NSError* error;
//                if ([url getResourceValue:&creationDate forKey:NSURLCreationDateKey error:&error]) {
//                    if (lastModifiedDate > creationDate) {
//                        lastModifiedDate = creationDate;
//                        lastModifiedFilePath = [url absoluteString];
//                    }
//                }
//                else {
//                    NSLog(@"No creation date");
//                }
//            }
//        }
    }
    
    NSLog(@"Lastest modified file: <%@>", lastModifiedFilePath);
    
//    NSString *documentsDirectory = [[[[NSFileManager defaultManager] URLsForDirectory:NSDocumentDirectory inDomains:NSUserDomainMask] lastObject] path];
//    NSArray *docFileList = [[NSFileManager defaultManager] subpathsAtPath:documentsDirectory];
//    NSEnumerator *docEnumerator = [docFileList objectEnumerator];
//    NSString *docFilePath;
//    NSDate *lastModifiedDate = [NSDate dateWithTimeIntervalSince1970:0];
//    NSString *lastModifiedFilePath = @"";
//
//    while ((docFilePath = [docEnumerator nextObject])) {
//        NSLog(docFilePath);
//        NSString *fullPath = [documentsDirectory stringByAppendingPathComponent:docFilePath];
//        NSDictionary *fileAttributes = [[NSFileManager defaultManager]  attributesOfItemAtPath:fullPath error:nil];
//        NSDate* currentModifiedDate = [fileAttributes fileModificationDate];
//
//        if (lastModifiedDate < currentModifiedDate) {
//            lastModifiedDate = lastModifiedDate;
//            lastModifiedFilePath = fullPath;
//        }
//    }
//
//    return lastModifiedFilePath;
    
    return nil;
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
//        if (! [imageToDisplayAbsPath isEqualToString:[NSString stringWithUTF8String:""]]) {
            [self performSelectorOnMainThread:@selector(setNewImage) withObject:imageToDisplayAbsPath waitUntilDone:YES];
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
    NSString *fileName = @"/Users/fredericrubio/Desktop/IMG_0113.png";
    NSImage *image = [[NSImage alloc] initWithContentsOfFile:fileName];
    if (image == nil) {
        NSLog(@"image nil");
    }
    else {
        NSLog(@"%@", image.name);
        NSLog(@"size %f %f",image.size.width,image.size.height);
    }
    image = nil;
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

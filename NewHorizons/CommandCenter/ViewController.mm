//
//  ViewController.m
//  CommandCenter
//
//  Created by Frédéric Rubio on 01/06/2019.
//  Copyright © 2019 Frédéric Rubio. All rights reserved.
//

#import "ViewController.h"

#include "NHOCommandCenter.hpp"

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
    
    NSThread* evtThread = [ [NSThread alloc] initWithTarget:self
                                                   selector:@selector( refreshCemaeraView )
                                                     object:nil ];
    
    [evtThread start];
    
    // manage Quit
    NSNotificationCenter* nc = [NSNotificationCenter defaultCenter];
    [nc addObserver:self
           selector:@selector(closeConnexion)
               name:NSApplicationWillTerminateNotification
             object:nil];
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
//
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

    for (NSURL *url in enumerator) {
        NSError *error;
        NSNumber *isDirectory = nil;
        if (! [url getResourceValue:&isDirectory forKey:NSURLIsDirectoryKey error:&error]) {
            // handle error
        }
        else if (! [isDirectory boolValue]) {
            // No error and it’s not a directory; do something with the file
//            NSLog(@"%@", [url absoluteString]);
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

- (void)refreshCemaeraView {
    
    // refresh network status
    if (self.networkStatus == false) {
        [self.network setImage:[NSImage imageNamed:@"Red.png"]];
    }
    
    [self getLatestsFile];
    
    while (self.keepRefreshing) {
        
        [NSThread sleepForTimeInterval:1];
        [self performSelectorOnMainThread:@selector(setNewImage) withObject:nil waitUntilDone:YES];
        
    }
    
    NSLog(@"End of refreshCemaeraView");

}

-(void) setNewImage {
    NSString *fileName = @"/Users/fredericrubio/Desktop/IMG_0113.png";
    NSImage *image = [[NSImage alloc] initWithContentsOfFile:fileName];
//    if (image == nil) {
//        NSLog(@"image nil");
//    }
//    else {
//        NSLog(@"%@", image.name);
//        NSLog(@"size %f %f",image.size.width,image.size.height);
//    }
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

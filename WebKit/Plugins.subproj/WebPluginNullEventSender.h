/*	
    IFPluginNullEventSender.h
	Copyright 2002, Apple, Inc. All rights reserved.
*/

#import <Foundation/Foundation.h>
#import <npapi.h>

@interface IFPluginNullEventSender : NSObject{
    NPP instance;
    NPP_HandleEventProcPtr NPP_HandleEvent;
    bool shouldStop;
}

-(id)initializeWithNPP:(NPP)pluginInstance functionPointer:(NPP_HandleEventProcPtr)HandleEventFunction;
-(void)sendNullEvents;
-(void)stop;
@end
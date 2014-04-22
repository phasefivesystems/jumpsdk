//
//  main.c
//  EchoChannelExample
//
//  Created by haseebq on 2014-04-22.
//  Copyright (c) 2014 Phase Five Systems. All rights reserved.
//

#include <stdio.h>


#include <stdio.h>
#include <unistd.h>
#include <stdarg.h>
#include <CoreFoundation/CoreFoundation.h>

#include "Jump_SDK.h"

void Log(const char* format, ...)
{
    va_list args;
    va_start (args, format);
    
    vprintf(format, args);
    printf("\r\n");
    
    va_end(args);
}

// Keeps track of statistics over all echo channels
typedef struct _GlobalEchoStats
{
    int totalChannelsOpened;
    int totalEchosSent;
} GlobalEchoStats;

// Keeps track of statistics about a single channel
typedef struct _EchoStats
{
    GlobalEchoStats* globalStats;
    int channelNumber;
    int numEchosSent;
} EchoStats;

// --------------------------------------------------------
JD_Result EchoVirtualChannelOpenFunc(const JD_VirtualChannelRef ref, void* openUserData)
{
    // Increment global stats
    GlobalEchoStats* globalStats = (GlobalEchoStats*)openUserData;
    globalStats->totalChannelsOpened++;
    
    // Allocate a new EchoStat structure for this channel and initialize it
    EchoStats* stats = (EchoStats*)malloc(sizeof(EchoStats));
    if (stats != NULL)
    {
        stats->globalStats = globalStats;
        stats->numEchosSent = 0;
        stats->channelNumber = globalStats->totalChannelsOpened;

        // Associate the channel with the EchoStat structure
        JD_VirtualChannelSetUserData(ref, stats);
        
        Log("Echo: New Channel Opened: %i", (int)stats->channelNumber);

        return JD_OK;
    }
    else
    {
        return JD_ERROR;
    }
}

JD_Result EchoVirtualChannelConsumeFunc(const JD_VirtualChannelRef ref, const void* data, uint32_t dataLen)
{
    EchoStats* stats = (EchoStats*)JD_VirtualChannelGetUserData(ref);
    
    stats->globalStats->totalEchosSent++;
    stats->numEchosSent++;
    
    Log("Echo: Channel:%i, sending echo #:%i", (int)stats->channelNumber, (int)stats->numEchosSent);
    
    JD_VirtualChannelSend(ref, data, dataLen);
    
    return JD_OK;
}

void EchoVirtualChannelCloseFunc(const JD_VirtualChannelRef ref)
{
    EchoStats* stats = (EchoStats*)JD_VirtualChannelGetUserData(ref);
    
    Log("Echo: Channel Closing: %i. Echos sent: %i", (int)stats->channelNumber, (int)stats->numEchosSent);

    // Cleanup per-channel stat
    free(stats);
}

// ----------------------------------------------------
int main(int argc, const char * argv[])
{
    JD_Result result;
    GlobalEchoStats echoStats;
    
    Log("Starting...");
    
    // Create a new client
    JD_ClientRef client = JD_ClientCreate();
    if (client == NULL)
    {
        Log("Could not create JD_Client");
        goto End;
    }
    
    // Register the virtual channel
    result = JD_ClientRegisterVirtualChannel(client,
                                             "JDECHO",
                                             JD_CHANNEL_FLAG_STATIC,
                                             &echoStats,
                                             EchoVirtualChannelOpenFunc,
                                             EchoVirtualChannelConsumeFunc,
                                             EchoVirtualChannelCloseFunc);
    if (result == JD_ERROR)
    {
        Log("Could not register virtual channel");
        goto End;
    }
    
    // The main thread _must_ process run loop mesages. Cocoa GUI apps do this by
    // default. If you have a command line / headless app  you must make sure the main
    // thread is processing runloop messages by calling CFRunLoopRun* functions
    while (1)
    {
        CFRunLoopRun();
    }
    
End:
    
    if (client != NULL)
    {
        JD_ClientDestroy(client);
    }
    
    return 0;
}

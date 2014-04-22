//
//  Jump_SDK.h
//
//  Jump Desktop Viewer SDK.
//  Copyright Phase Five Systems 2014
//
//

#ifndef JUMP_SDK_H
#define JUMP_SDK_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define JD_OK      0
#define JD_ERROR   1
    
typedef int JD_Result;
typedef void* JD_ClientRef;
typedef void* JD_VirtualChannelRef;

//
// Virtual channel callbacks
//
// Notes about threading: These callbacks happen on Jump SDK's processing thread. If you have
// global state, make sure you have appropriate locking in place to access state
//
    
typedef JD_Result (*JD_VirtualChannelOpenFunc)(const JD_VirtualChannelRef ref, void* openFuncData);
typedef JD_Result (*JD_VirtualChannelConsumeFunc)(const JD_VirtualChannelRef ref, const void* data, uint32_t dataLen);
typedef void (*JD_VirtualChannelCloseFunc)(const JD_VirtualChannelRef ref);
    
JD_ClientRef JD_ClientCreate();
void JD_ClientDestroy(JD_ClientRef client);
    

#define JD_CHANNEL_FLAG_STATIC   0
#define JD_CHANNEL_FLAG_DYNAMIC  1
    
JD_Result JD_ClientRegisterVirtualChannel(JD_ClientRef clientRef,
                                          const char* channelName,
                                          int channelFlags,
                                          void* openFuncData,
                                          JD_VirtualChannelOpenFunc openFunc,
                                          JD_VirtualChannelConsumeFunc consumeFunc,
                                          JD_VirtualChannelCloseFunc closeFunc);

void* JD_VirtualChannelGetUserData(JD_VirtualChannelRef virtualChannelRef);
void JD_VirtualChannelSetUserData(JD_VirtualChannelRef virtualChannelRef, void* userData);
JD_Result JD_VirtualChannelSend(JD_VirtualChannelRef virtualChannelRef, const void* data, uint32_t dataLen);
    
#ifdef __cplusplus
}
#endif

#endif

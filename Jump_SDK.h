//
//  Jump_SDK.h
//
//  Jump Desktop Viewer SDK.
//  Copyright Phase Five Systems 2014
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

// Flags for dataFlags in JD_VirtualChannelConsumeFunc
#define CHANNEL_FLAG_FIRST  0x1
#define CHANNEL_FLAG_LAST   0x2
#define CHANNEL_FLAG_MIDDLE 0
#define CHANNEL_FLAG_ONLY   (CHANNEL_FLAG_FIRST | CHANNEL_FLAG_LAST)
//
// Virtual channel callbacks
//
// Notes about threading: These callbacks happen on Jump SDK's processing thread. If you have
// global state, make sure you have appropriate locking in place to access state
//
    
//
// Callback to notify the plugin that a new virtual channel instance has been opened.
//
// @param ref           a reference to the new virtual channel
// @param openFuncData  user supplied callback parameter passed in when registering the channel via JD_ClientRegisterVirtualChannel
//
// @return              JD_OK if plugin can handle the channel open request
// @return              JD_ERROR to prevent the channel from opening
//
typedef JD_Result (*JD_VirtualChannelOpenFunc)(const JD_VirtualChannelRef ref, void* openFuncData);

//
// Callback to notify the plugin that data was received from the server over a virtual channel
//
// @param ref           a reference to the virtual channel
// @param data          pointer to the data
// @param dataLen       length in bytes of the data
// @param totalDataLen 	the length of the data written by a single write operation to the server end of the virtual channel
// @param dataFlags    	bit flags that provide information about the chunk of data being received:
//                          CHANNEL_FLAG_FIRST  - the first chunk of data
//                          CHANNEL_FLAG_LAST   - the last chunk of data
//                          CHANNEL_FLAG_MIDDLE - middle chunk of data (neither the first nor last)
//                          CHANNEL_FLAG_ONLY   - this is the first and last chunk of data
//
// @return              JD_OK if plugin successfully processed all the data
// @return              JD_ERROR if there was an error processing the data
//
typedef JD_Result (*JD_VirtualChannelConsumeFunc)(const JD_VirtualChannelRef ref, const void* data, uint32_t dataLen, uint32_t totalDataLen, uint32_t dataFlags);

//
// Callback to notify the plugin that a virtual channel was closed
//
// @param ref           a reference to the virtual channel. The plugin should make sure the virtual channel reference is no longer used after this function returns
//
typedef void (*JD_VirtualChannelCloseFunc)(const JD_VirtualChannelRef ref);

//
// Creates an initializes a new instance of the plugin framework.
//
// @return  a reference to the client or NULL if there was an error creating the client
//
JD_ClientRef JD_ClientCreate();
    
//
// Destroys the plugin framework and cleans up all resources
//
// @param client reference returned by an earlier call to JD_ClientCreate
//
void JD_ClientDestroy(JD_ClientRef client);
    

// Flag to indicate a static virtual channel
#define JD_CHANNEL_FLAG_STATIC   0
    
// Flag to indicate a dynamic virtual channel
#define JD_CHANNEL_FLAG_DYNAMIC  1
    
//
// Registers a new virtual channel with plugin framework
//
// @param clientRef     reference returned by an earlier call to JD_ClientCreate
// @param channelName   the name of the channel
// @param channelFlags  one of JD_CHANNEL_* flags
// @param openFuncData  optional value to pass to the openFun callback when a new channel is opened
// @param openFunc      pointer to a function called by the framework when a new channel is opened
// @param consumeFunc   pointer to a function called by the framework when it receives data from the server over a virtual channel
// @param closeFunc     pointer to a function called by the framework when a virtual channel is closed
JD_Result JD_ClientRegisterVirtualChannel(JD_ClientRef clientRef,
                                          const char* channelName,
                                          int channelFlags,
                                          void* openFuncData,
                                          JD_VirtualChannelOpenFunc openFunc,
                                          JD_VirtualChannelConsumeFunc consumeFunc,
                                          JD_VirtualChannelCloseFunc closeFunc);

//
// Sets a virtual channel's user data. You can retrive this data later by calling JD_VirtualChannelGetUserData.
//
// @param virtualChannelRef a reference to the virtual channel
// @param userData          the data
//
void JD_VirtualChannelSetUserData(JD_VirtualChannelRef virtualChannelRef, void* userData);

//
// Gets a virtual channel's user data set by an earlier call to JD_VirtualChannelSetUserData
//
// @param virtualChannelRef a reference to the virtual channel
//
// @return the user data
void* JD_VirtualChannelGetUserData(JD_VirtualChannelRef virtualChannelRef);
    
//
// Sends data to server over the virtual channel.
//
// @param virtualChannelRef a reference to the virtual channel
// @param data              the data to send
// @param dataLen           the length of data in bytes
//
// @return  JD_OK   if the data was successfully queued to be sent to the virtual channel
// @return  JD_ERROR if there was an error queuing the data
//
JD_Result JD_VirtualChannelSend(JD_VirtualChannelRef virtualChannelRef, const void* data, uint32_t dataLen);
    
#ifdef __cplusplus
}
#endif

#endif

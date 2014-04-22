Jump Desktop for Mac SDK
========================

Third party applications can use this SDK to implement RDP virtual channel plugins for Jump Desktop for Mac (JD).

Requirements
------------

* Requires Jump Desktop for Mac 4.1 or later. You can download a trial version from http://jumpdesktop.com/downloads/jdmac .
* XCode 5.0 or later.
* The SDK supports OSX 10.6+.


Architecture
------------

Jump Desktop for Mac has an out-of-process plugin model: plugins run as separate applications in the user's desktop session and communicate with JD for Mac using IPC. 

After the SDK is initialized it will try to register the virtual channel with an already running JD on the user's desktop. If JD isn't running, the SDK wait for JD to be launched by the user. 

Once registration is successful your plugin's virtual channel callbacks will be called the next time user connect to a RDP session. 

What this means is your app must always be running on the user's desktop for the virtual channel to be registered with JD. This is different from other plugin models where the hosting app runs the plugin code and registration only happens during app install. 

Getting Started
---------------

1. Drag and drop the following files into your XCode project:
    * Jump_SDK.h
    * libJumpApiClient.a
2. Select your project in Project Navigator and then select your app target.
3. Select that tab `Build Phases` and expand `Link Binary With Libraries`
4. Click `+` and then add `Foundation.framework`

Examples
--------
Example plugins are located in the Example directory.


Known issues
------------

The SDK does not work inside sandboxed applications. If you need support for sandboxed applications please let us know.
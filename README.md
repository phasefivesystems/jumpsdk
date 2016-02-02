Jump Desktop for Mac SDK
========================

Third party applications can use this SDK to implement RDP virtual channel plugins for [Jump Desktop for Mac](https://jumpdesktop.com/#jdmac) (JD).

Requirements
------------

* Requires Jump Desktop for Mac 5.0 or later. You can download the trial version from here: [Jump Desktop for Mac](https://jumpdesktop.com/#jdmac).
* XCode 5.0 or later.
* The SDK supports OSX 10.8+.


Architecture
------------

Jump Desktop for Mac has an out-of-process plugin model: plugins run as separate applications in the user's desktop session and communicate with JD for Mac using IPC. 

After the SDK is initialized it will try to register the virtual channel with an already running JD on the user's desktop. If JD isn't running, the SDK will wait for JD to be launched by the user. 

Once registration is successful, your plugin's virtual channel callbacks will be called the next time user connects to an RDP session. 

What this means is your app must always be running on the user's desktop for the virtual channel to be registered with JD. This is different from other plugin models where the hosting app runs the plugin code and registration happens during app install. 

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

The SDK does not work inside sandboxed applications. If your application needs to work inside OS X's sandbox please contact us: <support@jumpdesktop.com> 

Support
-----------

For support send an email to <support@jumpdesktop.com> or use the github issue tracker.

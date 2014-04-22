Example plugins
---------------

The Mac directory contains a virtual channel plugin for Jump Desktop for Mac called `EchoChannelExample`. `EchoChannelExample` implements the an echo server that echos back everything  sent to it over the virtual channel. 

The Win directory contains a Windows example application that uses the virtual channel created by EchoChannelExample to implement 'ping' functionality. The Windows app will send ping packets to the echo server, wait for the reply and calculate latency statistics. 

See the README in EchoChannelExample for build instructions. 